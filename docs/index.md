# API Documentation
<a name="top"/>

### Table of Contents

- [dictation_asr.proto](#dictation_asr.proto)
    - [Speech](#google.cloud.speech.v1.Speech)
    - [ConfigField](#google.cloud.speech.v1.ConfigField)
    - [LatticeEdge](#google.cloud.speech.v1.LatticeEdge)
    - [RecognitionAudio](#google.cloud.speech.v1.RecognitionAudio)
    - [RecognitionConfig](#google.cloud.speech.v1.RecognitionConfig)
    - [RecognitionLattice](#google.cloud.speech.v1.RecognitionLattice)
    - [RecognizeRequest](#google.cloud.speech.v1.RecognizeRequest)
    - [RecognizeResponse](#google.cloud.speech.v1.RecognizeResponse)
    - [SpeechContext](#google.cloud.speech.v1.SpeechContext)
    - [SpeechRecognitionAlternative](#google.cloud.speech.v1.SpeechRecognitionAlternative)
    - [SpeechRecognitionResult](#google.cloud.speech.v1.SpeechRecognitionResult)
    - [StreamingRecognitionConfig](#google.cloud.speech.v1.StreamingRecognitionConfig)
    - [StreamingRecognitionResult](#google.cloud.speech.v1.StreamingRecognitionResult)
    - [StreamingRecognizeRequest](#google.cloud.speech.v1.StreamingRecognizeRequest)
    - [StreamingRecognizeResponse](#google.cloud.speech.v1.StreamingRecognizeResponse)
    - [WordInfo](#google.cloud.speech.v1.WordInfo)
    - [RecognitionConfig.AudioEncoding](#google.cloud.speech.v1.RecognitionConfig.AudioEncoding)
    - [StreamingRecognitionResult.ResultFinalizationCause](#google.cloud.speech.v1.StreamingRecognitionResult.ResultFinalizationCause)
    - [StreamingRecognizeResponse.SpeechEventType](#google.cloud.speech.v1.StreamingRecognizeResponse.SpeechEventType)
  
- [Scalar Value Types](#scalar-value-types)



<a name="dictation_asr.proto"/>
<p align="right"><a href="#top">Top</a></p>

## dictation_asr.proto
Modified by Techmo, copyright by Google. Changes include:
1. Additions that introduce new features to the original API. Extensions (parts that were added to the original document) by Techmo are marked with [**Extension by Techmo**] tag.
   -   `MP3` audio encoding type.
   -   `ConfigField` as means to provide additional configuration.
   -   `ResultFinalizationCause` as means to indicate MRCPv2-related recognition result finalization cause.
   -   `RecognitionLattice` and `LatticeEdge` as means to return detailed recognition results.
2. Modifications of comments, according to how recognition is performed by Techmo.
   -   [*Unused*] tags for fields or values that are not used (ignored when provided in request, never returned in response).
   -   [*Unsupported*] tags for fields or values that will result in an error when provided in request.
3. Removal of `LongRunningRecognize` support (commented out).


<a name="google.cloud.speech.v1.Speech"/>

### Speech
Service that implements Google Cloud Speech API extended by Techmo.

| Method Name | Request Type | Response Type | Description |
| ----------- | ------------ | ------------- | ------------|
| Recognize | [RecognizeRequest](#google.cloud.speech.v1.RecognizeRequest) | [RecognizeResponse](#google.cloud.speech.v1.RecognizeRequest) | Performs synchronous speech recognition: receive results after all audio has been sent and processed. |
| StreamingRecognize | [StreamingRecognizeRequest](#google.cloud.speech.v1.StreamingRecognizeRequest) | [StreamingRecognizeResponse](#google.cloud.speech.v1.StreamingRecognizeRequest) | Performs bidirectional streaming speech recognition: receive results while sending audio. This method is only available via the gRPC API (not REST). |

 <!-- end services -->


<a name="google.cloud.speech.v1.ConfigField"/>

### ConfigField
[**Extension by Techmo**]
Provides a pair of configuration field name and value.


| Field | Type | Description |
| ----- | ---- | ----------- |
| key | [string](#string) | Name of configuration field. |
| value | [string](#string) | Value of configuration field. |






<a name="google.cloud.speech.v1.LatticeEdge"/>

### LatticeEdge
[**Extension by Techmo**]
Edge-specific information for recognition lattice.


| Field | Type | Description |
| ----- | ---- | ----------- |
| start_node | [int32](#int32) | Input node ID, node '0' is starting node for the lattice. |
| end_node | [int32](#int32) | End node ID. |
| symbol | [string](#string) | Word. |
| language_cost | [float](#float) | Language model cost. |
| acoustic_cost | [float](#float) | Raw acoustic score (unscaled). |
| duration | [int32](#int32) | Word duration in milliseconds. |






<a name="google.cloud.speech.v1.RecognitionAudio"/>

### RecognitionAudio
Contains audio data in the encoding specified in the `RecognitionConfig`.
Only `content` is allowed to be supplied.


| Field | Type | Description |
| ----- | ---- | ----------- |
| content | [bytes](#bytes) | The audio data bytes encoded as specified in `RecognitionConfig`. Note: as with all bytes fields, protobuffers use a pure binary representation, whereas JSON representations use base64. |
| uri | [string](#string) | [*Unsupported*] |






<a name="google.cloud.speech.v1.RecognitionConfig"/>

### RecognitionConfig
Provides information to the recognizer that specifies how to process the
request.


| Field | Type | Description |
| ----- | ---- | ----------- |
| encoding | [RecognitionConfig.AudioEncoding](#google.cloud.speech.v1.RecognitionConfig.AudioEncoding) | [*Required*] Encoding of audio data sent in all `RecognitionAudio` messages. |
| sample_rate_hertz | [int32](#int32) | [*Required*] Sample rate in Hertz of the audio data sent in all `RecognitionAudio` messages. Valid values are: 8000-48000. 16000 is optimal. For best results, set the sampling rate of the audio source to 16000 Hz. If that's not possible, use the native sample rate of the audio source (instead of re-sampling). [**Extension by Techmo**] Silently ignored for `FLAC`, `OGG_OPUS` and `MP3` encodings. Real sample rate will be detected from file header instead. |
| language_code | [string](#string) | [*Required*] The language of the supplied audio as a [BCP-47](https://www.rfc-editor.org/rfc/bcp/bcp47.txt) language tag. Example: "en-US". The only language supported at the moment is Polish (`pl-PL`). |
| max_alternatives | [int32](#int32) | [*Optional*] Maximum number of recognition hypotheses to be returned. Specifically, the maximum number of `SpeechRecognitionAlternative` messages within each `SpeechRecognitionResult`. The server may return fewer than `max_alternatives`. Valid values are `0`-`30`. A value of `0` or `1` will return a maximum of one. If omitted, will return a maximum of one. |
| profanity_filter | [bool](#bool) | [*Optional*][*Unused*] |
| speech_contexts | [SpeechContext](#google.cloud.speech.v1.SpeechContext)[] | [*Optional*] Array of SpeechContext. Allows to send a "context phrase" that switches language model for current recognition |
| enable_word_time_offsets | [bool](#bool) | [*Optional*] If `true`, the top result includes a list of words and the start and end time offsets (timestamps) for those words. If `false`, no word-level time offset information is returned. The default is `false`. |
| enable_automatic_punctuation | [bool](#bool) | [*Optional*][*Unused*] |
| config_fields | [ConfigField](#google.cloud.speech.v1.ConfigField) | [**Extension by Techmo**] [*Optional*] A means to provide additional configuration fields via request. |
| model | [string](#string) | [*Optional*][*Unused*] |
| use_enhanced | [bool](#bool) | [*Optional*][*Unused*] |




<a name="google.cloud.speech.v1.RecognitionLattice"/>

### RecognitionLattice
[**Extension by Techmo**]
Detailed recognition result (lattice).
Returned *only when requested* (`ConfigField`: return-lattice=true in `RecognitionConfig` Message), *only for final* (`is_final = true`) results, and *only when it's allowed by licence*.
When requested and not allowed by licence,
[google.rpc.Code.FAILED_PRECONDITION] will be returned.


| Field | Type | Description |
| ----- | ---- | ----------- |
| final_nodes | [int32](#int32) | List of final nodes. |
| edges | [LatticeEdge](#google.cloud.speech.v1.LatticeEdge) | List of lattice edges. |






<a name="google.cloud.speech.v1.RecognizeRequest"/>

### RecognizeRequest
The top-level message sent by the client for the `Recognize` method.


| Field | Type | Description |
| ----- | ---- | ----------- |
| config | [RecognitionConfig](#google.cloud.speech.v1.RecognitionConfig) | [*Required*] Provides information to the recognizer that specifies how to process the request. |
| audio | [RecognitionAudio](#google.cloud.speech.v1.RecognitionAudio) | [*Required*] The audio data to be recognized. |






<a name="google.cloud.speech.v1.RecognizeResponse"/>

### RecognizeResponse
The only message returned to the client by the `Recognize` method. It
contains the result as zero or more sequential `SpeechRecognitionResult`
messages.


| Field | Type | Description |
| ----- | ---- | ----------- |
| results | [SpeechRecognitionResult](#google.cloud.speech.v1.SpeechRecognitionResult) | [*Output only*] Sequential list of transcription results corresponding to sequential portions of audio. |






<a name="google.cloud.speech.v1.SpeechContext"/>

### SpeechContext


Context phrase that switches the model used during recognition. If the phrase correctly identifies the context model used in service, it will be used instead of the general model for the current recognition. Due to compatibility with Google API, the object is defined as a list of strings, but only the first element of the list is used as the context phrase, the rest are ignored if present.


| Field | Type | Description |
| ----- | ---- | ----------- |
| phrases | [string](#string)[] | First element of array contains "context phrase" which allows to use particular context model for the recognition |






<a name="google.cloud.speech.v1.SpeechRecognitionAlternative"/>

### SpeechRecognitionAlternative
Alternative hypotheses (a.k.a. n-best list).


| Field | Type | Description |
| ----- | ---- | ----------- |
| transcript | [string](#string) | [*Output only*] Transcript text representing the words that the user spoke. |
| confidence | [float](#float) | [*Output only*] The confidence estimate between 0.0 and 1.0. A higher number indicates an estimated greater likelihood that the recognized words are correct. |
| words | [WordInfo](#google.cloud.speech.v1.WordInfo) | [*Output only*] A list of word-specific information for each recognized word. |






<a name="google.cloud.speech.v1.SpeechRecognitionResult"/>

### SpeechRecognitionResult
A speech recognition result corresponding to a portion of the audio.


| Field | Type | Description |
| ----- | ---- | ----------- |
| alternatives | [SpeechRecognitionAlternative](#google.cloud.speech.v1.SpeechRecognitionAlternative) | [*Output only*] May contain one or more recognition hypotheses (up to the maximum specified in `max_alternatives`). These alternatives are ordered in terms of accuracy, with the top (first) alternative being the most probable, as ranked by the recognizer. |
| lattice | [RecognitionLattice](#google.cloud.speech.v1.RecognitionLattice) | [**Extension by Techmo**] [*Output only*] Detailed recognition result (lattice). Returned *only when requested* (`ConfigField`: return-lattice=true in `RecognitionConfig` Message), *only for final* (`is_final = true`) results, and *only when it's allowed by licence*. When requested and not allowed by licence, [google.rpc.Code.FAILED_PRECONDITION] will be returned. |






<a name="google.cloud.speech.v1.StreamingRecognitionConfig"/>

### StreamingRecognitionConfig
Provides information to the recognizer that specifies how to process the
request.


| Field | Type | Description |
| ----- | ---- | ----------- |
| config | [RecognitionConfig](#google.cloud.speech.v1.RecognitionConfig) | [*Required*] Provides information to the recognizer that specifies how to process the request. |
| single_utterance | [bool](#bool) | [*Optional*] If `false` or omitted, the recognizer will perform continuous recognition (continuing to wait for and process audio even if the user pauses speaking) until the client closes the input stream (gRPC API) or until the maximum time limit has been reached. May return multiple `StreamingRecognitionResult`s with the `is_final` flag set to `true`. If `true`, the recognizer will detect a single spoken utterance. When it detects that the user has paused or stopped speaking, it will return an `END_OF_SINGLE_UTTERANCE` event and cease recognition. It will return no more than one `StreamingRecognitionResult` with the `is_final` flag set to `true`. |
| interim_results | [bool](#bool) | [*Optional*] If `true`, interim results (tentative hypotheses) may be returned as they become available (these interim results are indicated with the `is_final=false` flag). If `false` or omitted, only `is_final=true` result(s) are returned. |






<a name="google.cloud.speech.v1.StreamingRecognitionResult"/>

### StreamingRecognitionResult
A streaming speech recognition result corresponding to a portion of the audio
that is currently being processed.


| Field | Type | Description |
| ----- | ---- | ----------- |
| alternatives | [SpeechRecognitionAlternative](#google.cloud.speech.v1.SpeechRecognitionAlternative) | [*Output only*] May contain one or more recognition hypotheses (up to the maximum specified in `max_alternatives`). These alternatives are ordered in terms of accuracy, with the top (first) alternative being the most probable, as ranked by the recognizer. |
| is_final | [bool](#bool) | [*Output only*] If `false`, this `StreamingRecognitionResult` represents an interim result that may change. If `true`, this is the final time the speech service will return this particular `StreamingRecognitionResult`, the recognizer will not return any further hypotheses for this portion of the transcript and corresponding audio. |
| stability | [float](#float) | [*Unused*] |
| result_finalization_cause | [StreamingRecognitionResult.ResultFinalizationCause](#google.cloud.speech.v1.StreamingRecognitionResult.ResultFinalizationCause) | [**Extension by Techmo**] [*Output only*] Indicates the cause of recognition result finalization. |
| lattice | [RecognitionLattice](#google.cloud.speech.v1.RecognitionLattice) | [**Extension by Techmo**] [*Output only*] Detailed recognition result (lattice). Returned *only when requested* (`ConfigField`: return-lattice=true in `RecognitionConfig` Message), *only for final* (`is_final = true`) results, and *only when it's allowed by licence*. When requested and not allowed by licence, [google.rpc.Code.FAILED_PRECONDITION] will be returned. |






<a name="google.cloud.speech.v1.StreamingRecognizeRequest"/>

### StreamingRecognizeRequest
The top-level message sent by the client for the `StreamingRecognize` method.
Multiple `StreamingRecognizeRequest` messages are sent. The first message
must contain a `streaming_config` message and must not contain `audio` data.
All subsequent messages must contain `audio` data and must not contain a
`streaming_config` message.


| Field | Type | Description |
| ----- | ---- | ----------- |
| streaming_config | [StreamingRecognitionConfig](#google.cloud.speech.v1.StreamingRecognitionConfig) | Provides information to the recognizer that specifies how to process the request. The first `StreamingRecognizeRequest` message must contain a `streaming_config` message. |
| audio_content | [bytes](#bytes) | The audio data to be recognized. Sequential chunks of audio data are sent in sequential `StreamingRecognizeRequest` messages. The first `StreamingRecognizeRequest` message must not contain `audio_content` data and all subsequent `StreamingRecognizeRequest` messages must contain `audio_content` data. The audio bytes must be encoded as specified in `RecognitionConfig`. Note: as with all bytes fields, protobuffers use a pure binary representation (not base64). |






<a name="google.cloud.speech.v1.StreamingRecognizeResponse"/>

### StreamingRecognizeResponse
`StreamingRecognizeResponse` is the only message returned to the client by
`StreamingRecognize`. A series of one or more `StreamingRecognizeResponse`
messages are streamed back to the client.

Here's an example of a series of ten `StreamingRecognizeResponse`s that might
be returned while processing audio:

1. results { alternatives { transcript: "tube" } stability: 0.01 }

2. results { alternatives { transcript: "to be a" } stability: 0.01 }

3. results { alternatives { transcript: "to be" } stability: 0.9 }
   results { alternatives { transcript: " or not to be" } stability: 0.01 }

4. results { alternatives { transcript: "to be or not to be"
                            confidence: 0.92 }
             alternatives { transcript: "to bee or not to bee" }
             is_final: true }

5. results { alternatives { transcript: " that's" } stability: 0.01 }

6. results { alternatives { transcript: " that is" } stability: 0.9 }
   results { alternatives { transcript: " the question" } stability: 0.01 }

7. results { alternatives { transcript: " that is the question"
                            confidence: 0.98 }
             alternatives { transcript: " that was the question" }
             is_final: true }

Notes:

- Only two of the above responses #4 and #7 contain final results; they are
  indicated by `is_final: true`. Concatenating these together generates the
  full transcript: "to be or not to be that is the question".

- The others contain interim `results`. #3 and #6 contain two interim
  `results`: the first portion has a high stability and is less likely to
  change; the second portion has a low stability and is very likely to
  change. A UI designer might choose to show only high stability `results`.

- The specific `stability` and `confidence` values shown above are only for
  illustrative purposes. Actual values may vary.

- In each response, only one of these fields will be set:
    `error`,
    `speech_event_type`, or
    one or more (repeated) `results`.


| Field | Type | Description |
| ----- | ---- | ----------- |
| error | [google.rpc.Status](#google.rpc.Status) | [*Output only*] If set, returns a [google.rpc.Status][google.rpc.Status] message that specifies the error for the operation. |
| results | [StreamingRecognitionResult](#google.cloud.speech.v1.StreamingRecognitionResult) | [*Output only*] This repeated list contains zero or more results that correspond to consecutive portions of the audio currently being processed. It contains zero or one `is_final=true` result (the newly settled portion), followed by zero or more `is_final=false` results (the interim results). |
| speech_event_type | [StreamingRecognizeResponse.SpeechEventType](#google.cloud.speech.v1.StreamingRecognizeResponse.SpeechEventType) | [*Output only*] Indicates the type of speech event. |






<a name="google.cloud.speech.v1.WordInfo"/>

### WordInfo
Word-specific information for recognized words. Word information is only
included in the response when certain request parameters are set, such
as `enable_word_time_offsets`.


| Field | Type | Description |
| ----- | ---- | ----------- |
| start_time | [google.protobuf.Duration](#google.protobuf.Duration) | [*Output only*] Time offset relative to the beginning of the audio, and corresponding to the start of the spoken word. This field is only set if `enable_word_time_offsets=true`. |
| end_time | [google.protobuf.Duration](#google.protobuf.Duration) | [*Output only*] Time offset relative to the beginning of the audio, and corresponding to the end of the spoken word. This field is only set if `enable_word_time_offsets=true`. |
| word | [string](#string) | [*Output only*] The word corresponding to this set of information. |





 <!-- end messages -->


<a name="google.cloud.speech.v1.RecognitionConfig.AudioEncoding"/>

### RecognitionConfig.AudioEncoding
The encoding of the audio data sent in the request.
All encodings support only 1 channel (mono) audio.

| Name | Number | Description |
| ---- | ------ | ----------- |
| ENCODING_UNSPECIFIED | 0 | Not specified. Will return result [google.rpc.Code.INVALID_ARGUMENT][google.rpc.Code.INVALID_ARGUMENT]. |
| LINEAR16 | 1 | Uncompressed 16-bit signed little-endian samples (Linear PCM). |
| FLAC | 2 | `FLAC` (Free Lossless Audio Codec) is the recommended encoding because it is lossless--therefore recognition is not compromised--and requires only about half the bandwidth of `LINEAR16`. `FLAC` stream encoding supports 16-bit and 24-bit samples, however, not all fields in `STREAMINFO` are supported. [**Extension by Techmo**] Supported only by `Recognize`. When requested by `StreamingRecognize`, will return result [google.rpc.Code.INVALID_ARGUMENT][google.rpc.Code.INVALID_ARGUMENT]. Silently ignores `sample_rate_hertz` and detects real sample rate from file header instead. |
| MULAW | 3 | [*Unsupported*] |
| AMR | 4 | [*Unsupported*] |
| AMR_WB | 5 | [*Unsupported*] |
| OGG_OPUS | 6 | Opus encoded audio frames in Ogg container ([OggOpus](https://wiki.xiph.org/OggOpus)). [**Extension by Techmo**] Silently ignores `sample_rate_hertz` and detects real sample rate from file header instead. |
| SPEEX_WITH_HEADER_BYTE | 7 | [*Unsupported*] |
| MP3 | 8 | [**Extension by Techmo**] `MP3` (standards ISO/IEC 11172-3 and ISO/IEC 13818-3) Only constant bit rate files are accepted. Silently ignores `sample_rate_hertz` and detects real sample rate from file header instead. |



<a name="google.cloud.speech.v1.StreamingRecognitionResult.ResultFinalizationCause"/>

### StreamingRecognitionResult.ResultFinalizationCause
[**Extension by Techmo**]
Indicates the cause of recognition result finalization. These are MRCPv2-related.
See [Completion-Cause](https://tools.ietf.org/html/rfc6787#section-9.4.11).

| Name | Number | Description |
| ---- | ------ | ----------- |
| RESULT_FINALIZATION_CAUSE_UNSPECIFIED | 0 | No recognition result finalization cause specified. |
| SUCCESS | 1 | Recognition has been finalized with a complete result after specified length of silence after user speech. See [Speech-Complete-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.15). |
| NO_INPUT_TIMEOUT | 2 | Recognition has started and there was no speech detected for a certain period of time. See [No-Input-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.6). |
| SUCCESS_MAXTIME | 3 | Recognition has been finalized because speech was too long, with a complete result. See [Recognition-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.7). |
| PARTIAL_MATCH | 4 | Recognition has been finalized with an incomplete result after specified length of silence after user speech. See [Speech-Incomplete-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.16). |
| NO_MATCH_MAXTIME | 5 | Recognition has been finalized because speech was too long, with no result. See [Recognition-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.7). |



<a name="google.cloud.speech.v1.StreamingRecognizeResponse.SpeechEventType"/>

### StreamingRecognizeResponse.SpeechEventType
Indicates the type of speech event.

| Name | Number | Description |
| ---- | ------ | ----------- |
| SPEECH_EVENT_UNSPECIFIED | 0 | No speech event specified. |
| END_OF_SINGLE_UTTERANCE | 1 | This event indicates that the server has detected the end of the user's speech utterance and expects no additional speech. Therefore, the server will not process additional audio (although it may subsequently return additional results). The client should stop sending additional audio data, half-close the gRPC connection, and wait for any additional results until the server closes the gRPC connection. This event is only sent if `single_utterance` was set to `true`, and is not used otherwise. |


 <!-- end enums -->

 <!-- end HasExtensions -->



<p align="right"><a href="#top">Top</a></p>
## Scalar Value Types

| .proto Type | Notes | C++ Type | Java Type | Python Type |
| ----------- | ----- | -------- | --------- | ----------- |
| <a name="double" /> double |  | double | double | float |
| <a name="float" /> float |  | float | float | float |
| <a name="int32" /> int32 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead. | int32 | int | int |
| <a name="int64" /> int64 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead. | int64 | long | int/long |
| <a name="uint32" /> uint32 | Uses variable-length encoding. | uint32 | int | int/long |
| <a name="uint64" /> uint64 | Uses variable-length encoding. | uint64 | long | int/long |
| <a name="sint32" /> sint32 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32 | int | int |
| <a name="sint64" /> sint64 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64 | long | int/long |
| <a name="fixed32" /> fixed32 | Always four bytes. More efficient than uint32 if values are often greater than 2^28. | uint32 | int | int |
| <a name="fixed64" /> fixed64 | Always eight bytes. More efficient than uint64 if values are often greater than 2^56. | uint64 | long | int/long |
| <a name="sfixed32" /> sfixed32 | Always four bytes. | int32 | int | int |
| <a name="sfixed64" /> sfixed64 | Always eight bytes. | int64 | long | int/long |
| <a name="bool" /> bool |  | bool | boolean | boolean |
| <a name="string" /> string | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string | String | str/unicode |
| <a name="bytes" /> bytes | May contain any arbitrary sequence of bytes. | string | ByteString | str |

