#include <sstream>
#include <atomic>
#include <thread>

#include <grpc++/grpc++.h>
#include <google/protobuf/text_format.h>

#include "cloud_speech_extended.grpc.pb.h"
#include "dictation_client.h"


namespace gsapi = ::google::cloud::speech::v1;

namespace techmo { namespace dictation {

void read_service_settings_option(const DictationClientConfig& config, gsapi::RecognitionConfig& recognition_config) {
    const auto& settings_string = config.service_settings;

    // split by ';'
    std::vector<std::string> settings_lines;
    //boost::split(settings_lines, settings_string, boost::is_any_of(";"));
    std::stringstream settings_stream(settings_string);
    std::string setting_line;
    while (std::getline(settings_stream, setting_line, ';')) {
        settings_lines.push_back(setting_line);
    }

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
            auto field = recognition_config.add_config_fields();
            field->set_key(key_value[0]);
            field->set_value(key_value[1]);
            std::cout << "key: " << field->key() << " | value: " << field->value() << std::endl;
        }
        else
        {
            std::cout << "Skipping invalid session settings line: " << line << std::endl;
        }
    }
}

void build_recognition_config(const DictationClientConfig& config, unsigned int sample_rate_hertz, gsapi::RecognitionConfig& recognition_config) {
    recognition_config.set_max_alternatives(config.max_alternatives);
    recognition_config.set_encoding(gsapi::RecognitionConfig_AudioEncoding_LINEAR16);
    recognition_config.set_sample_rate_hertz(sample_rate_hertz);
    recognition_config.set_language_code("pl-PL");
    recognition_config.set_enable_word_time_offsets(config.time_offsets);
    if (!config.service_settings.empty()) {
        read_service_settings_option(config, recognition_config);
    }
}

gsapi::RecognizeRequest build_sync_request(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) {
    gsapi::RecognizeRequest request;
    build_recognition_config(config, audio_sample_rate_hz, *request.mutable_config());
    request.mutable_audio()->set_content(audio_byte_content);

    return request;
}

std::vector<gsapi::StreamingRecognizeRequest> build_streaming_request(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content)
{
    gsapi::StreamingRecognizeRequest request;
    auto& request_streaming_config = *request.mutable_streaming_config();
    build_recognition_config(config, audio_sample_rate_hz, *request_streaming_config.mutable_config());
    request_streaming_config.set_single_utterance(config.single_utterance);
    request_streaming_config.set_interim_results(config.interim_results);

    std::vector<gsapi::StreamingRecognizeRequest> requests;
    requests.push_back(request);

    unsigned int frame_length = 20;//milliseconds [ms]
    unsigned int frame_size = frame_length * audio_sample_rate_hz / 1000;//samples
    for (auto i = 0; i < audio_byte_content.length(); i += frame_size)
    {
        gsapi::StreamingRecognizeRequest request;

        request.set_audio_content(audio_byte_content.substr(i,frame_size));
        requests.push_back(request);
    }

    return requests;
}

bool error_response(const gsapi::StreamingRecognizeResponse& response) {
    const auto is_error = response.has_error();

    if (is_error) {
        const auto& error = response.error();
        std::cerr << "Received error response: (" << std::to_string(error.code()) << ") " << error.message() << std::endl;
    }

    return is_error;
}

bool end_of_utterance(const gsapi::StreamingRecognizeResponse& response) {
    const auto is_eou = response.speech_event_type() == gsapi::StreamingRecognizeResponse_SpeechEventType_END_OF_SINGLE_UTTERANCE
            || response.speech_event_type() == gsapi::StreamingRecognizeResponse_SpeechEventType_NO_INPUT_TIMEOUT;

    if (is_eou) {
        std::cout << "Received end of utterance response." << std::endl;
    }

    return is_eou;
}

std::string protobuf_message_to_string(const google::protobuf::Message & message)
{
    grpc::string out_str;
    google::protobuf::TextFormat::PrintToString(message, &out_str);
    return out_str;
}


void DictationClient::Recognize(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) {
    grpc::ClientContext context;
    if (!config.session_id.empty()) {
        context.AddMetadata("session_id", config.session_id);
    }

    const gsapi::RecognizeRequest request = build_sync_request(config, audio_sample_rate_hz, audio_byte_content);

    gsapi::RecognizeResponse response;

    auto stub = gsapi::Speech::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    const grpc::Status status = stub->Recognize(&context, request, &response);

    if (status.ok()) {
        std::cout << protobuf_message_to_string(response) << std::endl;
    }
    else {
        std::cerr << "Recognize RPC failed with status " << status.error_code() << " " << status.error_message() << std::endl;
    }
}


void DictationClient::StreamingRecognize(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) {
    grpc::ClientContext context;
    if (!config.session_id.empty()) {
        context.AddMetadata("session_id", config.session_id);
    }

    auto stub = gsapi::Speech::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    auto stream = stub->StreamingRecognize(&context);

    const auto requests = build_streaming_request(config, audio_sample_rate_hz, audio_byte_content);

    const auto& config_request = requests.front();
    stream->Write(config_request);

    std::atomic<bool> half_closed_stream{false};

    std::thread writer([&half_closed_stream, &stream, &requests] {
        for (auto i = 1; i < requests.size(); ++i) {
            if (half_closed_stream or not stream->Write(requests[i])) {
                break;
            }
        }
        if (not half_closed_stream) {
            half_closed_stream = true;
            stream->WritesDone();
        }
    });

    const auto responses = [&half_closed_stream, &stream, &writer] {
        std::vector<gsapi::StreamingRecognizeResponse> streaming_received_responses;
        gsapi::StreamingRecognizeResponse streaming_received_response;
        while (stream->Read(&streaming_received_response)) {
            if (error_response(streaming_received_response) || end_of_utterance(streaming_received_response)) {
                if (not half_closed_stream) {
                    half_closed_stream = true;
                    stream->WritesDone();
                }
                writer.detach();
            }
            else {
                std::cout << protobuf_message_to_string(streaming_received_response) << std::endl;
            }
            streaming_received_responses.push_back(streaming_received_response);
        }
        if (writer.joinable()) {
            writer.join();
        }
        return streaming_received_responses;
    }();

    grpc::Status status = stream->Finish();

    if (!status.ok()) {
        std::cerr << "StreamingRecognize RPC failed with status " << status.error_code() << " " << status.error_message() << std::endl;
    }
}

}}
