import grpc
from service import dictation_asr_pb2 as dictation_asr_pb2

class ServiceUtils:

    @staticmethod
    def create_channel(address, tls_directory):
        if not tls_directory:
            return grpc.insecure_channel(address)

        def read_file(path):
            with open(path, 'rb') as file:
                return file.read()

        return grpc.secure_channel(address, grpc.ssl_channel_credentials(
            read_file(os.path.join(tls_directory, 'ca.crt')),
            read_file(os.path.join(tls_directory, 'client.key')),
            read_file(os.path.join(tls_directory, 'client.crt')),
        ))

    @staticmethod
    def check_health(channel, timeout):
        stub = health_pb2_grpc.HealthStub(channel)
        request = health_pb2.HealthCheckRequest()
        try:
            serving_status = stub.Check(request, timeout=timeout, wait_for_ready=True).status
            statusline = "service status: {}".format(health_pb2.HealthCheckResponse.ServingStatus.Name(serving_status))
        except grpc.RpcError as e:
            serving_status = health_pb2.HealthCheckResponse.ServingStatus.UNKNOWN
            statusline = "service status: UNKNOWN Received following RPC error from the service: [{}] {}".format(str(e.code()), str(e.details()))
        print(statusline)
        # NAGIOS return codes :
        # https://nagios-plugins.org/doc/guidelines.html#AEN78
        if serving_status == health_pb2.HealthCheckResponse.ServingStatus.SERVING:
            return 0
        elif serving_status == health_pb2.HealthCheckResponse.ServingStatus.NOT_SERVING:
            return 2
        return 3

    @staticmethod
    def build_recognition_config(sampling_rate, settings):
        recognition_config = dictation_asr_pb2.RecognitionConfig(
            encoding='LINEAR16',  # one of LINEAR16, FLAC, MULAW, AMR, AMR_WB
            sample_rate_hertz=sampling_rate,  # the rate in hertz
            # See https://g.co/cloud/speech/docs/languages for a list of supported languages.
            language_code='pl-PL',  # a BCP-47 language tag
            enable_word_time_offsets=settings.time_offsets(),  # if true, return recognized word time offsets
            max_alternatives=settings.max_alternatives(),  # maximum number of returned hypotheses
        )
        if (settings.context_phrase()):
            speech_context = recognition_config.speech_contexts.add()
            speech_context.phrases.append(settings.context_phrase())

        return recognition_config
