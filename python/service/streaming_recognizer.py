import threading
from . import dictation_asr_pb2
from . import dictation_asr_pb2_grpc
import grpc


class RequestIterator:
    """Thread-safe request iterator for streaming recognizer."""

    def __init__(self, audio_stream, settings):
        # Iterator data
        self.audio_stream = audio_stream
        self.audio_generator = self.audio_stream.generator()

        self.settings = settings

        self.request_builder = {
            True: self._config_request,
            False: self._normal_request
        }
        # Iterator state
        self.lock = threading.Lock()
        self.is_config_request = True
        self.eos = False  # indicates whether end of stream message was send (request to stop iterator)

    def _config_request(self):
        request = dictation_asr_pb2.StreamingRecognizeRequest(
            streaming_config=dictation_asr_pb2.StreamingRecognitionConfig(
                config=dictation_asr_pb2.RecognitionConfig(
                    encoding='LINEAR16',  # one of LINEAR16, FLAC, MULAW, AMR, AMR_WB
                    sample_rate_hertz=self.audio_stream.frame_rate(),
                    language_code='pl-PL',  # a BCP-47 language tag
                    max_alternatives=self.settings.max_alternatives(),
                    enable_word_time_offsets=self.settings.time_offsets(),  # if true, return recognized word time offsets
                ),
                single_utterance=True
            ),
            audio_content=dictation_asr_pb2.RecognitionAudio(
                content=next(self) #self.audio_generator
            )
        )

        # add additional service settings
        if self.settings.service_settings:
            settings_pairs = self.settings.service_settings.split(';')
            for settings_pair in settings_pairs:
                key_value = settings_pair.split('=')
                cf = request.config.additional_settings.add()
                cf.key = key_value[0]
                cf.value = key_value[1]

        # if self.settings.grammar_name:
        #     request.config.grammar_name = self.settings.grammar_name
        # elif self.settings.grammar:
        #     request.config.grammar_data = self.settings.grammar
        # else:
        #     raise ValueError("Grammar must be loaded or grammar name must be set first")

        self.is_config_request = False
        return request

    def _normal_request(self):
        data = next(self.audio_generator)
        return dictation_asr_pb2.RecognitionAudio(content=data)

    def __iter__(self):
        return self

    def __next__(self):
        with self.lock:
            return self.request_builder[self.is_config_request]()


class DictationRecognizer:

    def __init__(self, address):
        self.service = DictationRecognizer.connect(address)

    def recognize(self, audio_stream, settings):
        metadata = []
        if settings.session_id:
            metadata = [('session_id', settings.session_id())]
        requests_iterator = RequestIterator(audio_stream, settings)
        return self.service.StreamingRecognize(requests_iterator, metadata=metadata)
                                                                #, metadata=metadata

    # def define_grammar(self, grammar_name, grammar):
    #     request = dictation_asr_pb2.DefineGrammarRequest(grammar_name=grammar_name, grammar_data=grammar)
    #     response = self.service.DefineGrammar(request)
    #     return response

    @staticmethod
    def connect(endpoint):
        service = dictation_asr_pb2_grpc.SpeechStub(
            grpc.insecure_channel(endpoint))
        return service
