#include <iostream>

#include <boost/program_options.hpp>
#include <google/protobuf/text_format.h>

#include "audio-utils.h"
#include "dictation_client.h"
#include "VERSION.h"

namespace po = boost::program_options;

std::string ProtobufMessageToString(const google::protobuf::Message & message) {
    grpc::string out_str;
    google::protobuf::TextFormat::PrintToString(message, &out_str);
    return out_str;
}

std::map<std::string, std::string> ReadServiceSettingsOption(const std::string & settings_string) {
    std::map<std::string, std::string> service_settings;
    // split by ';'
    std::vector<std::string> settings_lines;
    //boost::split(settings_lines, settings_string, boost::is_any_of(";"));
    std::stringstream settings_stream(settings_string);
    std::string setting_line;
    while (std::getline(settings_stream, setting_line, ';')) {
        settings_lines.push_back(setting_line);
    }

    if (not settings_lines.empty()) {
        std::cout << "Passing session settings:" << std::endl;
        for (const auto & line : settings_lines)
        {
            // split by '='
            std::vector<std::string> key_value;
            //boost::split(key_value, line, boost::is_any_of("="));
            std::stringstream key_value_steam(line);
            std::string field;
            while (std::getline(key_value_steam, field, '=')) {
                key_value.push_back(field);
            }

            if (key_value.size() == 2)
            {
                const auto key = key_value[0];
                service_settings[key] = key_value[1];
                std::cout << "key: " << key << " | value: " << service_settings[key] << std::endl;
            }
            else
            {
                std::cout << "Skipping invalid session settings line: " << line << std::endl;
            }
        }
    }

    return service_settings;
}

techmo::dictation::DictationSessionConfig CreateDictationSessionConfig(const po::variables_map& userOptions) {
    techmo::dictation::DictationSessionConfig config;
    config.session_id = userOptions["session-id"].as<std::string>();
    config.grpc_timeout = userOptions["grpc-timeout"].as<int>();
    config.service_settings = ReadServiceSettingsOption(userOptions["service-settings"].as<std::string>());
    config.time_offsets = userOptions["time-offsets"].as<bool>();
    config.single_utterance = userOptions["single-utterance"].as<bool>();
    config.interim_results = userOptions["interim-results"].as<bool>();
    config.max_alternatives = userOptions["max-alternatives"].as<int>();
    config.context_phrase = userOptions["context-phrase"].as<std::string>();
    return config;
}

po::options_description CreateOptionsDescription(void) {
    // command line options
    po::options_description optionsDescription("Dictation ASR gRPC client options:");
    optionsDescription.add_options()
            ("help", "Print help message.")
            ("service-address", po::value<std::string>()->required(),
             "IP address and port (address:port) of a service the client will connect to.")
            ("tls-dir", po::value<std::string>()->default_value(""),
             "If set to a path with TLS/SSL credential files (client.crt, client.key, ca.crt), use TLS authentication. Otherwise use insecure channel (default).")
            ("audio-path", po::value<std::string>()->default_value(""),
             "Path to the audio file with speech to be recognized. It should be mono wav/ogg/mp3, 8kHz or 16kHz.")
            ("session-id", po::value<std::string>()->default_value(""),
             "Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.")
            ("grpc-timeout", po::value<int>()->default_value(0), "Timeout in milliseconds used to set gRPC deadline - "
             "how long the client is willing to wait for a reply from the server. "
             "If not specified, the service will set the deadline to a very large number.")
            ("wait-for-service-start", po::value<int>()->default_value(0),
             "Wait for the service start for a given duration in seconds. "
             "Additionally print service health status, but only for a non-zero timeout value. (defaults to 0)")
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
             "Maximum number of recognition hypotheses to be returned.")
            ("context-phrase", po::value<std::string>()->default_value(""),
             "Specifies which context model to use.");
    return optionsDescription;
}


int main(int argc, const char *const argv[]) {
    po::options_description optionsDescription(CreateOptionsDescription());
    po::variables_map userOptions;
    try {
        po::store(po::command_line_parser(argc, argv).options(optionsDescription).run(), userOptions);

        std::cout << "Dictation ASR gRPC client " << LIBDICTATION_CLIENT_VERSION << std::endl;

        if (userOptions.empty() or userOptions.count("help")) {
            std::cout << optionsDescription;
            return 0;
        }

        po::notify(userOptions);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cout << optionsDescription;
        return 1;
    }

    try {
        techmo::dictation::DictationSessionConfig config = CreateDictationSessionConfig(userOptions);

        techmo::dictation::DictationClient dictation_client{
            userOptions["service-address"].as<std::string>(),
            userOptions["tls-dir"].as<std::string>(),
        };

        auto wait_for_service_timeout = userOptions["wait-for-service-start"].as<int>();
        if (wait_for_service_timeout > 0) {
            auto health_status = dictation_client.CheckHealth(wait_for_service_timeout);
            /* NAGIOS return codes :
             * https://nagios-plugins.org/doc/guidelines.html#AEN78 */
            if (health_status == health::HealthCheckResponse::NOT_SERVING)
                return 2;
            else if (health_status != health::HealthCheckResponse::SERVING)
                return 3;
        }

        std::string audio_path = userOptions["audio-path"].as<std::string>();

        if (audio_path.empty())
        {
            return 0;
        }

        WAV_DATA wav_data;

        if(audio_path.substr(audio_path.find_last_of(".") + 1) == "wav") {
            wav_data = ReadWaveFile(audio_path);
        } 
        else if(audio_path.substr(audio_path.find_last_of(".") + 1) == "ogg") {
            wav_data = ReadOggFile(audio_path);
        } 
        else {
            std::cout << "Error: not supported audio file. Only *.wav and *.ogg files are supported." << std::endl;
            return 1;
        }

        if (userOptions.count("streaming")) {
            const auto responses = dictation_client.StreamingRecognize(config, wav_data);

            for (const auto& response : responses) {
                std::cout << ProtobufMessageToString(response) << std::endl;
            }
        }
        else {
            const gsapi::RecognizeResponse response = dictation_client.Recognize(config, wav_data);

            std::cout << ProtobufMessageToString(response) << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
