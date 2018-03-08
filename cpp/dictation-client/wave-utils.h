#ifndef __WAVE_UTILS_H__
#define __WAVE_UTILS_H__


#include <string>


/// WAVE file header structure
/// Note: Header metadata are not supported.
struct WAV_HEADER//from sarmata/utils/WaveFile.h
{
    char               RIFF[4];        // RIFF Header      Magic header
    unsigned int       chunkSize;      // RIFF Chunk Size
    char               WAVE[4];        // WAVE Header
    char               fmt[4];         // FMT header
    unsigned int       subchunk1Size;  // Size of the fmt chunk
    unsigned short     audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short     numOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned int       samplesPerSec;  // Sampling Frequency in Hz
    unsigned int       bytesPerSec;    // bytes per second
    unsigned short     blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short     bitsPerSample;  // Number of bits per sample
    char               subchunk2ID[4]; // "data"  string
    unsigned int       subchunk2Size;  // Sampled data length
};

/// WAVE file data structure: header and audio bytes
struct WAV_DATA
{
    WAV_HEADER header;
    std::string audioBytes;
};

/// Reads at given path WAVE file header into WAV_HEADER struct and audio bytes into std::string.
WAV_DATA ReadWaveFile(const std::string & wavePath);

/// Writes at given path WAVE file with given sample rate [Hz] and audio bytes as std::string.
void WriteWaveFile(const std::string & wavePath, unsigned int sampleRate, const std::string & audioBytes);


#endif /* __WAVE_UTILS_H__ */
