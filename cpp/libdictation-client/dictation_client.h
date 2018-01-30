#ifndef __DICTATION_CLIENT_H__
#define __DICTATION_CLIENT_H__

#include "dictation_asr.grpc.pb.h"


namespace gsapi = google::cloud::speech::v1;

namespace techmo { namespace dictation {

struct DictationClientConfig {
    std::string session_id = "";        // Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
    bool time_offsets = false;          // If true, returns also recognized word time offsets.
    bool single_utterance = true;       // Whether to perform continuous recognition (false) or stop after single utterance when detected pause in speech (true, default).
    bool interim_results = false;       // Whether to receive interim results (true) or not (false, default).
    std::string service_settings = "";  // Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request.
    int max_alternatives = 1;           // Maximum number of recognition hypotheses to be returned.
};

class DictationClient {
public:
    DictationClient(const std::string& service_address) : service_address_{ service_address } {}

    gsapi::RecognizeResponse Recognize(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content);

    std::vector<gsapi::StreamingRecognizeResponse> StreamingRecognize(const DictationClientConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content);

private:
    DictationClient(); // Disable default constructor.

    const std::string service_address_;    // IP address and port (address:port) of a service the client will connect to.
};

}}


#endif //__DICTATION_CLIENT_H__
