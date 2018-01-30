#include <fstream>
#include <cstring>

#include "wave-utils.h"


WAV_DATA ReadWaveFile(const std::string & wavePath)
{
    static_assert(sizeof(WAV_HEADER) == 44, "sizeof(WAV_HEADER) is not equal to 44, disable alignment");
    std::fstream wav_file(wavePath, std::ios::binary | std::ios::in);
    WAV_DATA wave;
    wav_file.read((char*)(&wave.header), sizeof(wave.header));
    if (wave.header.subchunk2ID[0] != 'd' || wave.header.subchunk2ID[1] != 'a' || wave.header.subchunk2ID[2] != 't' || wave.header.subchunk2ID[3] != 'a') {
        throw std::runtime_error{"Waves with metadata are not supported."};
    }
    wave.audioBytes = std::string(wave.header.subchunk2Size,' ');
    wav_file.read((char*)(&wave.audioBytes[0]), wave.audioBytes.size());
    return wave;
}

void WriteWaveFile(const std::string & wavePath, unsigned int sampleRate, const std::string & audioBytes)
{
    std::fstream wave_file(wavePath, std::ios::binary | std::ios::trunc | std::ios::out);

    WAV_HEADER _header;
    memcpy(_header.RIFF, "RIFF", 4);
    _header.chunkSize = 36 + 0;
    memcpy(_header.WAVE, "WAVE", 4);
    memcpy(_header.fmt, "fmt ", 4);
    _header.subchunk1Size = 16;
    _header.audioFormat = 1;
    _header.numOfChan = 1;
    _header.samplesPerSec = sampleRate;
    _header.bytesPerSec = sampleRate * 1 * 16/8;
    _header.blockAlign = 1 * 16/8;
    _header.bitsPerSample = 16;
    memcpy(_header.subchunk2ID, "data", 4);
    _header.subchunk2Size = 0;
    _header.subchunk2Size = (unsigned)audioBytes.size();
    _header.chunkSize = 36 + _header.subchunk2Size;

    wave_file.write((char*)(&_header), sizeof(_header));
    wave_file.write((char*)&audioBytes[0], audioBytes.size());
    wave_file.flush();
}
