#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "dictation_client.h"
#include "VERSION.h"


namespace po = boost::program_options;

/// WAVE file header structure
struct WAV_HEADER//from sarmata/utils/WaveFile.h
{
    char               RIFF[4];        // RIFF Header      Magic header
    unsigned int       ChunkSize;      // RIFF Chunk Size
    char               WAVE[4];        // WAVE Header
    char               fmt[4];         // FMT header
    unsigned int       Subchunk1Size;  // Size of the fmt chunk
    unsigned short     AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short     NumOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned int       SamplesPerSec;  // Sampling Frequency in Hz
    unsigned int       bytesPerSec;    // bytes per second
    unsigned short     blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short     bitsPerSample;  // Number of bits per sample
    char               Subchunk2ID[4]; // "data"  string
    unsigned int       Subchunk2Size;  // Sampled data length
};

void read_wave_file(const std::string & wave_path, WAV_HEADER & wave_header, std::string & wave_content)
{
    static_assert(sizeof(WAV_HEADER) == 44, "sizeof(WAV_HEADER) is not equal to 44, disable alignment");
    std::fstream wav_file(wave_path, std::ios::binary | std::ios::in);
    wav_file.read((char*)(&wave_header), sizeof(wave_header));
    if (wave_header.Subchunk2ID[0] != 'd' || wave_header.Subchunk2ID[1] != 'a' || wave_header.Subchunk2ID[2] != 't' || wave_header.Subchunk2ID[3] != 'a') {
        throw std::runtime_error{"Waves with metadata are not supported."};
    }
    wave_content = std::string(wave_header.Subchunk2Size,' ');
    wav_file.read((char*)(&wave_content[0]), wave_content.size());
}

po::options_description CreateHelpOptions(void) {
    // command line options
    po::options_description helpOptions("Dictation ASR gRPC client options:");
    helpOptions.add_options()
            ("help", "Print help message.")
            ("service-address", po::value<std::string>(),
             "IP address and port (address:port) of a service the client will connect to.")
            ("wav-path", po::value<std::string>(),
             "Path to wave file with audio content to be sent to service via RPC.")
            ("session-id", po::value<std::string>()->default_value(""),
             "Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.")
            ("streaming", "If present, will perform asynchronous RPC.")
            ("time-offsets", po::value<bool>()->default_value(false),
             "If true, returns also recognized word time offsets.")
            ("single-utterance", po::value<bool>()->default_value(true),
             "Whether to perform continuous recognition (false) or stop after single utterance when detected pause in speech (true, default).")
            ("interim-results", po::value<bool>()->default_value(false),
             "Whether to receive interim results (true) or not (false, default).")
            ("service-settings", po::value<std::string>()->default_value(""),
             "Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request")
            ("max-alternatives", po::value<int>()->default_value(1),
             "Maximum number of recognition hypotheses to be returned.");
    return helpOptions;
}

int main(int argc, const char *const argv[]) {
    po::options_description helpOptions(CreateHelpOptions());
    po::variables_map userOptions;
    try {
        po::store(po::command_line_parser(argc, argv).options(helpOptions).run(), userOptions);
        po::notify(userOptions);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (userOptions.count("help")) {
        std::cout << helpOptions;
        return 0;
    }

    std::cout << "Dictation ASR gRPC client " << LIBDICTATION_CLIENT_VERSION << std::endl;

    if (!userOptions.count("wav-path") || !userOptions.count("service-address")) {//bad usage
        std::cerr << "Usage: " << argv[0] << " --wav-path /path/to/audio.wav --service-address host:address" << std::endl;
        return 1;
    }

    try {
        techmo::dictation::DictationClientConfig config;
        config.session_id = userOptions["session-id"].as<std::string>();
        config.time_offsets = userOptions["time-offsets"].as<bool>();
        config.single_utterance = userOptions["single-utterance"].as<bool>();
        config.interim_results = userOptions["interim-results"].as<bool>();
        config.service_settings = userOptions["service-settings"].as<std::string>();
        config.max_alternatives = userOptions["max-alternatives"].as<int>();

        WAV_HEADER wav_header;
        std::string audio_byte_content;
        read_wave_file(userOptions["wav-path"].as<std::string>(), wav_header, audio_byte_content);

        techmo::dictation::DictationClient dictation_client{ userOptions["service-address"].as<std::string>() };

        if (userOptions.count("streaming")) {
            dictation_client.StreamingRecognize(config, wav_header.SamplesPerSec, audio_byte_content);
        }
        else {
            dictation_client.Recognize(config, wav_header.SamplesPerSec, audio_byte_content);
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
