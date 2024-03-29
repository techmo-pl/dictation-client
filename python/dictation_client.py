#!/usr/bin/python3
import sys
from argparse import ArgumentParser
from utils.service_utils import ServiceUtils
from utils.audio_stream import AudioStream
from utils.audio_loader import AudioLoader
from utils.mic_stream import MicrophoneStream
from service.dictation_settings import DictationSettings
from service.streaming_recognizer import StreamingRecognizer
from service.sync_recognizer import SyncRecognizer
from VERSION import __version__


def print_results(results):
    for res in results:
        print()
        print("{}".format(res['transcript']))
        words = res['transcript'].split()
        ali = res['alignment']
        if len(words) == len(ali):
            for i in range(0, len(words)):
                time = ali[i]
                if len(time) > 0:
                    print("{} [{}.{:02d} - {}.{:02d}]".format(words[i], time[0].seconds, int(time[0].nanos / 10000000),
                                                          time[1].seconds, int(time[1].nanos / 10000000)))

def create_audio_stream(args):
    # create audio file stream
    if args.audio is not None:
        return AudioStream(args.audio, args.frame_length, args.delay)

    # create microphone stream
    if args.mic:
        rate = 16000  # [Hz]
        chunk = int(rate / 10)  # [100 ms]
        return MicrophoneStream(rate, chunk)

    # default
    raise ValueError("Unknown media source to create")


if __name__ == '__main__':
    print("Dictation ASR gRPC client " + __version__)

    parser = ArgumentParser(allow_abbrev=False)
    parser.add_argument("--service-address", "-s", dest="address", required=True,
                        help="IP address and port (address:port) of a service the client will connect to.", type=str)
    parser.add_argument("--audio-path", "-a", dest="audio",
                        help="Path to the audio file with speech to be recognized. It should be mono wav/ogg/mp3, 8kHz or 16kHz.")
    parser.add_argument("--mic", help="Use microphone as an audio source (instead of audio file).", action='store_true')
    parser.add_argument("--tls-dir", dest="tls_directory", default="",
                        help="If set to a path with TLS/SSL credential files (client.crt, client.key, ca.crt), use TLS authentication. Otherwise use insecure channel (default).", type=str)
    parser.add_argument("--session-id",
                        help="Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.",
                        default="", type=str)
    parser.add_argument("--grpc-timeout",
                        help="Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the server. If not specified, the service will set the deadline to a very large number.",
                        default=0, type=int)
    parser.add_argument("--wait-for-service-start", help="Wait for the service start for a given duration in seconds. Additionally print service health status, but only for a non-zero timeout value. (defaults to 0)", default=0, type=int)
    # request configuration section
    parser.add_argument("--context-phrase", help="Specifies which context model to use.", default="", type=str)
    parser.add_argument("--max-alternatives", help="Maximum number of recognition hypotheses to be returned.",
                        default=1, type=int)
    parser.add_argument("--time-offsets", help="If set - the recognizer will return also word time offsets.",
                        action="store_true", default=False)
    parser.add_argument("--single-utterance", help="If set - the recognizer will detect a single spoken utterance.",
                        action="store_true", default=False)
    parser.add_argument("--interim-results", help="If set - messages with interim results will be shown.",
                        action="store_true", default=False)
    parser.add_argument("--frame-length",  dest="frame_length", help="The length of single audio frame in [ms] for audio file source. Used mainly for testing purposes.",
                        default=20, type=int)
    parser.add_argument("--sync", help="If present, will perform synchronous RPC instead of asynchronous (streaming) call. It is not recommended to use this option for large files. For audio larger than 3.5MB, recognition quality is degraded - for the best possible recognition, send shorter audio fragments or use the streaming mode.",
                        action="store_true", default=False)
    parser.add_argument("--delay", help="Delay between sending requests [ms]. Set it equal to frame_length for real time simulation.", 
                        default=0, type=int,)
    # timeouts
    parser.add_argument("--no-input-timeout", help="MRCP v2 no input timeout [ms].", default=5000, type=int)
    parser.add_argument("--speech-complete-timeout", help="MRCP v2 speech complete timeout [ms].", default=2000,
                        type=int)
    parser.add_argument("--speech-incomplete-timeout", help="MRCP v2 speech incomplete timeout [ms].", default=4000,
                        type=int)
    parser.add_argument("--recognition-timeout", help="MRCP v2 recognition timeout [ms].", default=10000, type=int)
    

    # Stream audio to the ASR engine and print all hypotheses to standard output
    args = parser.parse_args()

    settings = DictationSettings(args)
    channel = ServiceUtils.create_channel(args.address, args.tls_directory)

    if args.wait_for_service_start > 0:
        health_status = ServiceUtils.check_health(channel, args.wait_for_service_start)
        if health_status != 0:
            sys.exit(health_status)

    if args.sync:
        recognizer = SyncRecognizer(channel, settings)

        if args.audio is not None:
            audio = AudioLoader(args.audio)
            print('Recognizing...')
            results = recognizer.recognize(audio)
            print_results(results)

    else:
        if args.max_alternatives > 1 and not args.single_utterance:
            print("Continuous Streaming Recognition doesn't support multiple ASR alternatives.\n"
            "Set --max-alternatives to 1 or run streaming recognition with --single-utterance parameter.")
            sys.exit(1)

        recognizer = StreamingRecognizer(channel, settings)    

        if args.audio is not None or args.mic:
            with create_audio_stream(args) as stream:
                print('Recognizing...')
                results = recognizer.recognize(stream)
                print_results(results)
