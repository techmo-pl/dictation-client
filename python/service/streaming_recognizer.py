import os
import threading
from . import dictation_asr_pb2 as dictation_asr_pb2
from . import dictation_asr_pb2_grpc as dictation_asr_pb2_grpc
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
    def __init__(self, address, ssl_directory, settings_args):
        # Use ArgumentParser to parse settings
        self.service = dictation_asr_pb2_grpc.SpeechStub(StreamingRecognizer.create_channel(address, ssl_directory))
        self.settings = settings_args

    def recognize(self, audio):
        requests_iterator = RequestIterator(audio, self.settings)
        return self.recognize_audio_content(requests_iterator)

    def recognize_audio_content(self, requests_iterator):
        time_offsets = self.settings.time_offsets()

        timeout=None
        if self.settings.grpc_timeout() > 0:
            timeout = self.settings.grpc_timeout() / 1000 # milliseconds to seconds
        metadata = []
        if self.settings.session_id():
            metadata = [('session_id', self.settings.session_id())]

        recognitions = self.service.StreamingRecognize(requests_iterator, timeout=timeout, metadata=metadata)

        confirmed_results = []
        alignment = []
        confidence = 1.0

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
                if first.is_final:
                    if time_offsets:
                        for word in first.alternatives[0].words:
                            if word.word != '<eps>':
                                confirmed_results.append(word.word)
                                alignment.append([word.start_time, word.end_time])
                    else:
                        confirmed_results.append(first.alternatives[0].transcript)
                    confidence = min(confidence, first.alternatives[0].confidence)
                else:
                    print(u"Temporal results - {}".format(first))

        # build final results
        final_alignment = [[]]
        final_transc = ' '.join(confirmed_results)

        if time_offsets and alignment:
            final_alignment = alignment

        return [{
            'transcript': final_transc,
            'alignment': final_alignment,
            'confidence': confidence
        }]  # array with one element

    @staticmethod
    def create_channel(address, ssl_directory):
        if not ssl_directory:
            return grpc.insecure_channel(address)

        def read_file(path):
            with open(path, 'rb') as file:
                return file.read()

        return grpc.secure_channel(address, grpc.ssl_channel_credentials(
            read_file(os.path.join(ssl_directory, 'ca.crt')),
            read_file(os.path.join(ssl_directory, 'client.key')),
            read_file(os.path.join(ssl_directory, 'client.crt')),
        ))

    @staticmethod
    def build_recognition_config(sampling_rate, settings):
        recognition_config = dictation_asr_pb2.RecognitionConfig(
            encoding='LINEAR16',  # one of LINEAR16, FLAC, MULAW, AMR, AMR_WB
            sample_rate_hertz=sampling_rate,  # the rate in hertz
            # See https://g.co/cloud/speech/docs/languages for a list of supported languages.
            language_code='pl-PL',  # a BCP-47 language tag
            enable_word_time_offsets=settings.time_offsets(),  # if true, return recognized word time offsets
            max_alternatives=1,  # maximum number of returned hypotheses
        )
        if (settings.context_phrase()):
            speech_context = recognition_config.speech_contexts.add()
            speech_context.phrases.append(settings.context_phrase())

        return recognition_config

    @staticmethod
    def build_configuration_request(sampling_rate, settings):
        config_req = dictation_asr_pb2.StreamingRecognizeRequest(
            streaming_config=dictation_asr_pb2.StreamingRecognitionConfig(
                config=StreamingRecognizer.build_recognition_config(sampling_rate, settings),
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
