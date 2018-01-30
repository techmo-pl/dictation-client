#include <iostream>

#include <boost/program_options.hpp>
#include <google/protobuf/text_format.h>

#include "wave-utils.h"
#include "dictation_client.h"
#include "VERSION.h"


namespace po = boost::program_options;

std::string protobuf_message_to_string(const google::protobuf::Message & message)
{
    grpc::string out_str;
    google::protobuf::TextFormat::PrintToString(message, &out_str);
    return out_str;
}

po::options_description CreateOptionsDescription(void) {
    // command line options
    po::options_description optionsDescription("Dictation ASR gRPC client options:");
    optionsDescription.add_options()
            ("help", "Print help message.")
            ("service-address", po::value<std::string>(),
             "IP address and port (address:port) of a service the client will connect to.")
            ("wav-path", po::value<std::string>(),
             "Path to wave file with audio content to be sent to service via RPC.")
            ("session-id", po::value<std::string>()->default_value(""),
             "Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.")
            ("streaming", "If present, will perform asynchronous RPC. This is obligatory for audio content larger than 3.5 MB.")
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
    return optionsDescription;
}

int main(int argc, const char *const argv[]) {
    po::options_description optionsDescription(CreateOptionsDescription());
    po::variables_map userOptions;
    try {
        po::store(po::command_line_parser(argc, argv).options(optionsDescription).run(), userOptions);
        po::notify(userOptions);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (userOptions.count("help")) {
        std::cout << optionsDescription;
        return 0;
    }

    std::cout << "Dictation ASR gRPC client " << LIBDICTATION_CLIENT_VERSION << std::endl;

    if (not userOptions.count("wav-path") || not userOptions.count("service-address")) {//bad usage
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

        const auto wave = ReadWaveFile(userOptions["wav-path"].as<std::string>());

        techmo::dictation::DictationClient dictation_client{ userOptions["service-address"].as<std::string>() };

        if (userOptions.count("streaming")) {
            const auto responses = dictation_client.StreamingRecognize(config, wave.header.samplesPerSec, wave.audioBytes);

            for (const auto& response : responses) {
                std::cout << protobuf_message_to_string(response) << std::endl;
            }
        }
        else {
            const gsapi::RecognizeResponse response = dictation_client.Recognize(config, wave.header.samplesPerSec, wave.audioBytes);

            std::cout << protobuf_message_to_string(response) << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
