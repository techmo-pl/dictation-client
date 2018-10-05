Python implementation of Dictation ASR gRPC client.

[Optional] To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

To run:
 - Use Python 3.5 with virtual environment and install required packages:
```
virtualenv -p python3.5 venv
source venv/bin/activate
pip install -r requirements.txt
```

Run:
```
python dictation_client.py --service-address "192.168.1.1:4321" --wave-path audio.wav
```

Usage:
```
usage: dictation_client.py [-h] --service-address ADDRESS [--wave-path WAVE]
                           [--mic] [--session-id SESSION_ID]
                           [--max-alternatives MAX_ALTERNATIVES]
                           [--time-offsets] [--single-utterance]
                           [--interim-results]
                           [--no-input-timeout NO_INPUT_TIMEOUT]
                           [--speech-complete-timeout SPEECH_COMPLETE_TIMEOUT]
                           [--speech-incomplete-timeout SPEECH_INCOMPLETE_TIMEOUT]
                           [--recognition-timeout RECOGNITION_TIMEOUT]

optional arguments:
  -h, --help            show this help message and exit
  --service-address ADDRESS
                        IP address and port (address:port) of a service the
                        client will connect to.
  --wave-path WAVE      Path to wave file with speech to be recognized. Should
                        be mono, 8kHz or 16kHz.
  --mic                 Use microphone as an audio source (instead of wave
                        file).
  --session-id SESSION_ID
                        Session ID to be passed to the service. If not
                        specified, the service will generate a default session
                        ID itself.
  --max-alternatives MAX_ALTERNATIVES
                        Maximum number of recognition hypotheses to be
                        returned.
  --time-offsets        If set - the recognizer will return also word time
                        offsets.
  --single-utterance    If set - the recognizer will detect a single spoken
                        utterance.
  --interim-results     If set - messages with temporal results will be shown.
  --no-input-timeout NO_INPUT_TIMEOUT
                        MRCP v2 no input timeout [ms].
  --speech-complete-timeout SPEECH_COMPLETE_TIMEOUT
                        MRCP v2 speech complete timeout [ms].
  --speech-incomplete-timeout SPEECH_INCOMPLETE_TIMEOUT
                        MRCP v2 speech incomplete timeout [ms].
  --recognition-timeout RECOGNITION_TIMEOUT
                        MRCP v2 recognition timeout [ms].
```

