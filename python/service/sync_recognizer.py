import os
import threading
from grpc_health.v1 import health_pb2
from grpc_health.v1 import health_pb2_grpc
from . import dictation_asr_pb2 as dictation_asr_pb2
from . import dictation_asr_pb2_grpc as dictation_asr_pb2_grpc
from utils.audio_loader import AudioLoader
from utils.service_utils import ServiceUtils


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
                    config=ServiceUtils.build_recognition_config(audio.sampling_rate_hz, self.settings),
                    audio=dictation_asr_pb2.RecognitionAudio(content=audio.audio_data),
                )
            )
        except grpc.RpcError as error:
            raise error
        
        recognition = response.results[0]

        # process response
        results = []

        alternatives=min(max_alternatives, len(recognition.alternatives))

        if max_alternatives > 1:
            print("Number of alternative recognitions:", alternatives)

        for i in range(alternatives):

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
