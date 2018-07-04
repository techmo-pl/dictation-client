#!/usr/bin/python3
from argparse import ArgumentParser
from utils.audio_source import AudioStream
from utils.mic_source import MicrophoneStream
from service.dictation_settings import DictationSettings
from service.streaming_recognizer import DictationRecognizer
from VERSION import DICTATION_CLIENT_VERSION


def create_audio_stream(args):
    # create wave file stream
    if args.wave is not None:
        return AudioStream(args.wave)

    # create microphone stream
    if args.mic:
        rate = 16000  # [Hz]
        chunk = int(rate / 10)  # [100 ms]
        return MicrophoneStream(rate, chunk)

    # default
    raise ValueError("Unknown media source to create")


# def print_results(results):
#
#     for res in results:
#         print("{}".format(res['transcript']))
#         words = res['transcript'].split()
#         ali = res['alignment']
#         if len(words) == len(ali):
#             for i in range(0, len(words)):
#                 time = ali[i]
#                 print("{} [{}.{:02d} - {}.{:02d}]".format(words[i], time[0].seconds, int(time[0].nanos / 10000000),
#                                                           time[1].seconds, int(time[1].nanos / 10000000)))

def print_results(responses, stream):
    if responses is None:
        print("Empty results - None object")
        return

    for response in responses:
        if response is None:
            print("Empty results - skipping response")
            continue

        print("Received response with status: {}".format(response['transcript']))

        if response.error:
            print("[ERROR]: {}".format(response.error))

        # single response expected
        processing_completed = True
        if not response.results.is_final:
            processing_completed = False

        if processing_completed:
            stream.close()

        n = 1
        for res in response.results.alternatives:
            # transcript = " ".join([word.transcript for word in res.words])
            print("[{}.] {} ({})".format(n, res.transcript, res.confidence))
            n += 1


if __name__ == '__main__':
    print("Dictation ASR gRPC client " + DICTATION_CLIENT_VERSION)

    parser = ArgumentParser()
    parser.add_argument("--service-address", dest="address", required=True, help="IP address and port (address:port) of a service the client will connect to.", type=str)
    parser.add_argument("--wave-path", dest="wave", help="Path to wave file with speech to be recognized. Should be mono, 8kHz or 16kHz.")
    parser.add_argument("--session-id", help="Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.", default="", type=str)
    parser.add_argument("--mic", help="Use microphone as an audio source (instead of wave file).", action='store_true')
    # request configuration section
    parser.add_argument("--time-offsets", help="If set - the recognizer will return also word time offsets.", action="store_true", default=False)
    parser.add_argument("--single-utterance", help="If set - the recognizer will detect a single spoken utterance.", action="store_true", default=False)
    parser.add_argument("--interim-results", help="If set - messages with temporal results will be shown.", action="store_true", default=False)
    parser.add_argument("--max-alternatives", help="Maximum number of recognition hypotheses to be returned. Default 1", action="store_true", default=1)
    # timeouts
    parser.add_argument("--no-input-timeout", help="MRCP v2 no input timeout [ms].", default=5000, type=int)
    parser.add_argument("--speech-complete-timeout", help="MRCP v2 speech complete timeout [ms].", default=2000, type=int)
    parser.add_argument("--recognition-timeout", help="MRCP v2 recognition timeout [ms].", default=10000, type=int)

    
    # Stream audio to the ASR engine and print all hypotheses to standard output
    args = parser.parse_args()
    settings = DictationSettings(args)
    recognizer = DictationRecognizer(args.address)

    # --------------------------
    # recognize section
    # --------------------------
    if args.wave is not None or args.mic:
        #validate_recognition_settings(settings)

        with create_audio_stream(args) as stream:

            results = recognizer.recognize(stream, settings)
            print_results(results, stream)
