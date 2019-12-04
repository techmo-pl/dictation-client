#include <sstream>
#include <atomic>
#include <thread>

#include <grpc++/grpc++.h>

#include "dictation_asr.grpc.pb.h"
#include "dictation_client.h"


namespace techmo { namespace dictation {

// Forward declarations
void prepare_context(grpc::ClientContext& context, const DictationSessionConfig& config);
gsapi::RecognizeRequest build_sync_request(const DictationSessionConfig& config, const std::string& audio_byte_content);
std::vector<gsapi::StreamingRecognizeRequest> build_streaming_request(const DictationSessionConfig& config, const std::string& audio_byte_content);
bool error_response(const gsapi::StreamingRecognizeResponse& response);
bool end_of_utterance(const gsapi::StreamingRecognizeResponse& response);
std::string grpc_status_to_string(const grpc::Status& status);


gsapi::RecognizeResponse DictationClient::Recognize(DictationSessionConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) const {
    config.audio_sample_rate_hz = audio_sample_rate_hz;
    const gsapi::RecognizeRequest request = build_sync_request(config, audio_byte_content);

    gsapi::RecognizeResponse response;

    auto stub = gsapi::Speech::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    grpc::ClientContext context;
    prepare_context(context, config);

    const grpc::Status status = stub->Recognize(&context, request, &response);

    if (not status.ok()) {
        std::cerr << "Recognize RPC failed with status " << grpc_status_to_string(status) << std::endl;
    }

    return response;
}


std::vector<gsapi::StreamingRecognizeResponse> DictationClient::StreamingRecognize(DictationSessionConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) const {
    auto stub = gsapi::Speech::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    grpc::ClientContext context;
    prepare_context(context, config);

    auto stream = stub->StreamingRecognize(&context);

    config.audio_sample_rate_hz = audio_sample_rate_hz;
    const auto requests = build_streaming_request(config, audio_byte_content);

    const auto& config_request = requests.front();
    stream->Write(config_request);

    // When received an error response from the server, the server will not process
    // additional audio (although it may subsequently return additional results).
    // The client should stop sending additional audio, half-close the gRPC connection,
    // and wait for any additional results until the server closes the gRPC connection.
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
            }
            else {
                std::cout << "Received response." << std::endl;
            }
            streaming_received_responses.push_back(streaming_received_response);
        }
        if (writer.joinable()) {
            writer.join();
        }
        return streaming_received_responses;
    }();

    const grpc::Status status = stream->Finish();

    if (not status.ok()) {
        std::cerr << "StreamingRecognize RPC failed with status " << grpc_status_to_string(status) << std::endl;
    }

    return responses;
}


void prepare_context(grpc::ClientContext& context, const DictationSessionConfig& config) {
    if (not config.session_id.empty()) {
        context.AddMetadata("session_id", config.session_id);
    }
    if (config.grpc_timeout > 0) {
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds{ config.grpc_timeout });
    }
}

void fill_additional_settings(const DictationSessionConfig& config, gsapi::RecognitionConfig& recognition_config) {
    for (const auto& entry : config.service_settings) {
        auto field = recognition_config.add_config_fields();
        field->set_key(entry.first);
        field->set_value(entry.second);
    }
}

void build_recognition_config(const DictationSessionConfig& config, gsapi::RecognitionConfig& recognition_config) {
    recognition_config.set_sample_rate_hertz(config.audio_sample_rate_hz);
    recognition_config.set_max_alternatives(config.max_alternatives);
    recognition_config.set_encoding(config.encoding);
    recognition_config.set_language_code(config.language_code);
    recognition_config.set_enable_word_time_offsets(config.time_offsets);
    if (not config.service_settings.empty()) {
        fill_additional_settings(config, recognition_config);
    }
    if (!config.context_phrase.empty()) {
        gsapi::SpeechContext* speech_context = recognition_config.add_speech_contexts();
        speech_context->add_phrases(context_phrase);
    }
}

gsapi::RecognizeRequest build_sync_request(const DictationSessionConfig& config, const std::string& audio_byte_content) {
    gsapi::RecognizeRequest request;
    build_recognition_config(config, *request.mutable_config());
    request.mutable_audio()->set_content(audio_byte_content);

    return request;
}

std::vector<gsapi::StreamingRecognizeRequest> build_streaming_request(const DictationSessionConfig& config, const std::string& audio_byte_content)
{
    gsapi::StreamingRecognizeRequest request;
    auto& request_streaming_config = *request.mutable_streaming_config();
    build_recognition_config(config, *request_streaming_config.mutable_config());
    request_streaming_config.set_single_utterance(config.single_utterance);
    request_streaming_config.set_interim_results(config.interim_results);

    std::vector<gsapi::StreamingRecognizeRequest> requests;
    requests.push_back(request);

    unsigned int frame_length = 20;//milliseconds [ms]
    unsigned int frame_size = frame_length * config.audio_sample_rate_hz / 1000;//samples
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
    const auto is_eou = response.speech_event_type() == gsapi::StreamingRecognizeResponse_SpeechEventType_END_OF_SINGLE_UTTERANCE;

    if (is_eou) {
        std::cout << "Received end of utterance response." << std::endl;
    }

    return is_eou;
}

std::string grpc_status_to_string(const grpc::Status& status) {
    // Status codes and their use in gRPC explanation can be found here:
    // https://github.com/grpc/grpc/blob/master/doc/statuscodes.md
    // https://grpc.io/grpc/cpp/namespacegrpc.html#aff1730578c90160528f6a8d67ef5c43b
    const std::string status_string = [&status]() {
        using code = grpc::StatusCode;
        switch (status.error_code()) {
        // Based on https://grpc.io/grpc/cpp/impl_2codegen_2status__code__enum_8h_source.html
            case code::OK: return "OK";
            case code::CANCELLED: return "CANCELLED";
            case code::UNKNOWN: return "UNKNOWN";
            case code::INVALID_ARGUMENT: return "INVALID_ARGUMENT";
            case code::DEADLINE_EXCEEDED: return "DEADLINE_EXCEEDED";
            case code::NOT_FOUND: return "NOT_FOUND";
            case code::ALREADY_EXISTS: return "ALREADY_EXISTS";
            case code::PERMISSION_DENIED: return "PERMISSION_DENIED";
            case code::UNAUTHENTICATED: return "UNAUTHENTICATED";
            case code::RESOURCE_EXHAUSTED: return "RESOURCE_EXHAUSTED";
            case code::FAILED_PRECONDITION: return "FAILED_PRECONDITION";
            case code::ABORTED: return "ABORTED";
            case code::OUT_OF_RANGE: return "OUT_OF_RANGE";
            case code::UNIMPLEMENTED: return "UNIMPLEMENTED";
            case code::INTERNAL: return "INTERNAL";
            case code::UNAVAILABLE: return "UNAVAILABLE";
            case code::DATA_LOSS: return "DATA_LOSS";
            case code::DO_NOT_USE: return "DO_NOT_USE";
            default: return "Status code not recognized";
        }
    }();

    return status_string + " (" + std::to_string(status.error_code()) + ") " + status.error_message();
}

}}
