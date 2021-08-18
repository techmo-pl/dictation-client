#ifndef __DICTATION_CLIENT_H__
#define __DICTATION_CLIENT_H__

#include "dictation_asr.grpc.pb.h"

#include "../dictation-client/wave-utils.h"


namespace gsapi = google::cloud::speech::v1;

namespace techmo { namespace dictation {

struct DictationSessionConfig {
    std::string session_id = "";        // Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
                                        // Session ID is the best way to match log's from client application with these on server side.
    int grpc_timeout = 0;               // Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the server.
    std::map<std::string, std::string> service_settings; // A map of <key, value> pairs defining settings to be sent to service via gRPC request.
    bool time_offsets = false;          // If true, returns also recognized word time offsets.
    bool single_utterance = true;       // Whether to perform continuous recognition (false) or stop after single utterance when detected pause in speech (true, default).
    bool interim_results = false;       // Whether to receive interim results (true) or not (false, default).
    gsapi::RecognitionConfig_AudioEncoding encoding = gsapi::RecognitionConfig_AudioEncoding_LINEAR16;
                                        // Encoding of audio data.
    unsigned int audio_sample_rate_hz = 0;       // Sampling frequency in hertz of audio data.
    unsigned int bytes_per_sec = 0;     // Bytes per second
    std::string language_code = "pl-PL";// The language of the supplied audio as a BCP-47 language tag.
    int max_alternatives = 1;           // Maximum number of recognition hypotheses to be returned.
    std::string context_phrase;         // Specifies which context model to use.
};

class DictationClient {
public:
    DictationClient(const std::string& service_address, const std::string& tls_directory)
        : service_address_{ service_address }
        , tls_directory_{ tls_directory }
    {
    }

    gsapi::RecognizeResponse Recognize(DictationSessionConfig& config, const WAV_DATA& wav_data) const;

    std::vector<gsapi::StreamingRecognizeResponse> StreamingRecognize(DictationSessionConfig& config, const WAV_DATA& wav_data) const;

private:
    DictationClient(); // Disable default constructor.

    const std::string service_address_;    // IP address and port (address:port) of a service the client will connect to.
    const std::string tls_directory_;
};

}}


#endif //__DICTATION_CLIENT_H__
