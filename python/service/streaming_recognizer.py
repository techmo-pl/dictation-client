import os
import threading
from grpc_health.v1 import health_pb2
from grpc_health.v1 import health_pb2_grpc
from . import dictation_asr_pb2 as dictation_asr_pb2
from . import dictation_asr_pb2_grpc as dictation_asr_pb2_grpc
from utils.service_utils import ServiceUtils
import grpc



class RequestIterator:
    """Thread-safe request iterator for streaming recognizer."""

    def __init__(self, audio_stream, settings):
        # Iterator data
        self.audio_stream = audio_stream
        self.audio_generator = self.audio_stream.generator()

        self.settings = settings

        self.request_builder = {
            True: self._initial_request,
            False: self._normal_request
        }
        # Iterator state
        self.lock = threading.Lock()
        self.is_initial_request = True
        self.eos = False  # indicates whether end of stream message was send (request to stop iterator)

    def _initial_request(self):
        req = StreamingRecognizer.build_configuration_request(self.audio_stream.frame_rate(), self.settings)
        self.is_initial_request = False
        return req

    def _normal_request(self):
        data = next(self.audio_generator)
        if data == None:
            raise StopIteration

        return dictation_asr_pb2.StreamingRecognizeRequest(audio_content=data)

    def __iter__(self):
        return self

    def __next__(self):
        with self.lock:
            return self.request_builder[self.is_initial_request]()


class StreamingRecognizer:
    def __init__(self, channel, settings_args):
        self.channel = channel
        self.settings = settings_args

    def recognize(self, audio):
        self.service = dictation_asr_pb2_grpc.SpeechStub(self.channel)
        requests_iterator = RequestIterator(audio, self.settings)
        return self.recognize_audio_content(requests_iterator)

    def recognize_audio_content(self, requests_iterator):
        time_offsets = self.settings.time_offsets()
        max_alternatives = self.settings.max_alternatives()

        timeout=None
        if self.settings.grpc_timeout() > 0:
            timeout = self.settings.grpc_timeout() / 1000 # milliseconds to seconds
        metadata = []
        if self.settings.session_id():
            metadata = [('session_id', self.settings.session_id())]

        recognitions = self.service.StreamingRecognize(requests_iterator, timeout=timeout, metadata=metadata)

        results = []

        for recognition in recognitions:
            if recognition.error.code:
                print(u"Received error response: ({}) {}".format(recognition.error.code, recognition.error.message))
                requests_iterator.audio_stream.close()

            elif recognition.speech_event_type != dictation_asr_pb2.StreamingRecognizeResponse.SPEECH_EVENT_UNSPECIFIED:
                print(u"Received speech event type: {}".format(
                    dictation_asr_pb2.StreamingRecognizeResponse.SpeechEventType.Name(recognition.speech_event_type)))
                requests_iterator.audio_stream.close()

            # process response type
            elif recognition.results is not None and len(recognition.results) > 0:
                first = recognition.results[0]

                alternatives = min(max_alternatives, len(first.alternatives))

                if max_alternatives > 1:
                    print("Number of alternative recognitions:", alternatives)

                for i in range(alternatives):

                    confirmed_results = []
                    alignment = []
                    confidence = 1.0
                    final_transc = ""

                    if first.is_final:
                        if time_offsets:
                            for word in first.alternatives[i].words:
                                if word.word != '<eps>':
                                    confirmed_results.append(word.word)
                                    alignment.append([word.start_time, word.end_time])
                                    final_transc = ' '.join(confirmed_results)
                        else:
                            confirmed_results = first.alternatives[i].transcript
                            final_transc = confirmed_results
                        confidence = min(confidence, first.alternatives[i].confidence)
                    else:
                        print(u"Interim results - {}".format(first))
   
                    # build final results
                    final_alignment = [[]]  

                    if time_offsets and alignment:
                        final_alignment = alignment
                    
                    single_result={
                        'transcript': final_transc,
                        'alignment': final_alignment,
                        'confidence': confidence,
                    }
                    results.append(single_result)

        return results

    @staticmethod
    def build_configuration_request(sampling_rate, settings):
        config_req = dictation_asr_pb2.StreamingRecognizeRequest(
            streaming_config=dictation_asr_pb2.StreamingRecognitionConfig(
                config=ServiceUtils.build_recognition_config(sampling_rate, settings),
                single_utterance=settings.single_utterance(),
                interim_results=settings.interim_results()
            )
            # no audio data in first request (config only)
        )
        # timeout settings
        timeouts = settings.timeouts_map()
        for settings_key in timeouts:
            cf = config_req.streaming_config.config.config_fields.add()
            cf.key = settings_key
            cf.value = "{}".format(timeouts[settings_key])

        return config_req
