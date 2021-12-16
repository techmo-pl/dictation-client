#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "audio-utils.h"



WAV_DATA ReadWaveFile(const std::string & audioPath){

    static_assert(sizeof(WAV_HEADER) == 44, "sizeof(WAV_HEADER) is not equal to 44, disable alignment");
    std::fstream wav_file(audioPath, std::ios::binary | std::ios::in);
    if (!wav_file.is_open()) {
        throw std::runtime_error{"Could not open file " + audioPath + " !"};
    }
    WAV_DATA wave;
    wav_file.read((char*)(&wave.header), sizeof(wave.header));
    if (wave.header.numOfChan != 1){
        throw std::runtime_error{"Error: not supported audio file. Only waves with single channel (mono) are supported."};
    }
    if (wave.header.subchunk2ID[0] != 'd' || wave.header.subchunk2ID[1] != 'a' || wave.header.subchunk2ID[2] != 't' || wave.header.subchunk2ID[3] != 'a') {
        throw std::runtime_error{"Error: not supported audio file. Waves with metadata are not supported."};
    }
    wave.audioBytes = std::string(wave.header.subchunk2Size,' ');
    wav_file.read((char*)(&wave.audioBytes[0]), wave.audioBytes.size());
    return wave;
}


WAV_DATA ReadOggFile(const std::string & audioPath){

    OggVorbis_File vf;

    if(ov_fopen(audioPath.c_str(), &vf) < 0) {
        throw std::runtime_error{"Invalid Ogg file!"};
    }

    vorbis_info* info = ov_info(&vf, -1);
    if(info->channels!=1){
	    throw std::runtime_error{"Error: not supported audio file. Only single channel (mono) audio is supported."};
    }
            
    int word_size = 2; //16-bit samples

    std::vector<char> audio_data;
    int current_section = 0;
    long bytes;
    long total_read = 0;
    char buffer[4096];
    do {
        bytes = ov_read(&vf, buffer, 4096, 0, word_size, 1, &current_section);
        if (bytes < 0) {
            throw std::runtime_error{"Invalid Ogg file! Error while reading audio data."};
        }
        else {
            total_read += bytes;
            audio_data.insert(audio_data.end(), buffer, buffer + bytes);
        }
    } while (bytes > 0);

    std::string audioBytes(audio_data.begin(), audio_data.end());

    // create WAV_WAV_HEADER struct instance
    WAV_HEADER header = CreateWaveHeader(info->rate, audioBytes);

    // create WAV_DATA struct instance
    WAV_DATA wav_data
    {
        header,
        audioBytes,
    };

    // close vorbis file
    ov_clear(&vf);

    return wav_data;
}


WAV_HEADER CreateWaveHeader(unsigned int sampleRate, const std::string & audioBytes){

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
    _header.subchunk2Size = (unsigned)audioBytes.size();
    _header.chunkSize = 36 + _header.subchunk2Size;

    return _header;
}


void WriteWaveFile(const std::string & audioPath, unsigned int sampleRate, const std::string & audioBytes){

    std::fstream wave_file(audioPath, std::ios::binary | std::ios::trunc | std::ios::out);

    WAV_HEADER _header = CreateWaveHeader(sampleRate, audioBytes);

    wave_file.write((char*)(&_header), sizeof(_header));
    wave_file.write((char*)&audioBytes[0], audioBytes.size());
    wave_file.flush();
}
