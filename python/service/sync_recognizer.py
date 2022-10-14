import os
import threading
from grpc_health.v1 import health_pb2
from grpc_health.v1 import health_pb2_grpc
from . import dictation_asr_pb2 as dictation_asr_pb2
from . import dictation_asr_pb2_grpc as dictation_asr_pb2_grpc
from utils.audio_loader import AudioLoader
from utils.audio_utils import AudioUtils


import grpc


class SyncRecognizer:
    def __init__(self, channel, settings_args):
        self.channel = channel
        self.settings = settings_args


    def recognize(self, audio):

        time_offsets = self.settings.time_offsets()
        max_alternatives =  self.settings.max_alternatives()

        try:
            self.service = dictation_asr_pb2_grpc.SpeechStub(self.channel)
            response = self.service.Recognize(
                dictation_asr_pb2.RecognizeRequest(
                    config=AudioUtils.build_recognition_config(audio.sampling_rate_hz, self.settings),
                    audio=dictation_asr_pb2.RecognitionAudio(content=audio.audio_data),
                )
            )
        except grpc.RpcError as error:
            raise error
        
        recognition = response.results[0]

        # process response
        results = []

        for i in range(max_alternatives):

            confirmed_results = []
            alignment = []
            confidence = 1.0
            final_transc = ""

            if time_offsets:
                for word in recognition.alternatives[i].words:
                    if word.word != '<eps>':
                        confirmed_results.append(word.word)
                        alignment.append([word.start_time, word.end_time])
                        final_transc = ' '.join(confirmed_results)
            else:
                confirmed_results = recognition.alternatives[i].transcript
                final_transc = confirmed_results
            confidence = min(confidence, recognition.alternatives[i].confidence)
   
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



    #################################################
    # def recognize(
    #     self, audio: AudioLoader
    # ) -> dictation_asr_pb2.RecognizeResponse:
    #     try:
    #         self.service = dictation_asr_pb2_grpc.SpeechStub(self.channel)
    #         response = self.service.Recognize(
    #             dictation_asr_pb2.RecognizeRequest(
    #                 config=AudioUtils.build_recognition_config(audio.sampling_rate_hz, settings),
    #                 audio=dictation_asr_pb2.RecognitionAudio(content=audio.audio_data),
    #             )
    #         )
    #     except grpc.RpcError as error:
    #         raise error
    #     return response
    ################################################

    # def make_config(
    #     self, audio: audio_loader.AudioLoader
    # ) -> dictation_asr_pb2.RecognitionConfig:
    #     config = dictation_asr_pb2.RecognitionConfig(
    #         encoding='LINEAR16',  # one of LINEAR16, FLAC, MULAW, AMR, AMR_WB
    #         sample_rate_hertz=sampling_rate,  # the rate in hertz
    #         # See https://g.co/cloud/speech/docs/languages for a list of supported languages.
    #         language_code='pl-PL',  # a BCP-47 language tag
    #         enable_word_time_offsets=settings.time_offsets(),  # if true, return recognized word time offsets
    #         max_alternatives=1,  # maximum number of returned hypotheses
    #     )
    #     if (settings.context_phrase()):
    #         speech_context = recognition_config.speech_contexts.add()
    #         speech_context.phrases.append(settings.context_phrase())
    #     return config


    #     @staticmethod
    # def build_configuration_request(sampling_rate, settings):
    #     config_req = dictation_asr_pb2.StreamingRecognizeRequest(
    #         streaming_config=dictation_asr_pb2.StreamingRecognitionConfig(
    #             config=AudioUtils.build_recognition_config(sampling_rate, settings),
    #             single_utterance=settings.single_utterance(),
    #             interim_results=settings.interim_results()
    #         )
    #         # no audio data in first request (config only)
    #     )
    #     # timeout settings
    #     timeouts = settings.timeouts_map()
    #     for settings_key in timeouts:
    #         cf = config_req.streaming_config.config.config_fields.add()
    #         cf.key = settings_key
    #         cf.value = "{}".format(timeouts[settings_key])

    #     return config_req

