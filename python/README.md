# Intro
Python implementation of Dictation ASR gRPC client.

[Optional] To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

# Running
To run:
 - Use Python 3.5 with virtual environment and install required packages:
```
virtualenv -p python3.5 venv
source venv/bin/activate
pip install -r requirements.txt
```

Run:
```
python dictation_client.py --service-address "185.110.50.61:62075" --wave-path audio.wav
```

# Troubleshooting
A set of common errors may occur.  Below are the steps to fix some of the issues.

## Python3.5 does not exist

### Problem
```shell script
virtualenv -p python3.5 venv
The path python3.5 (from --python=python3.5) does not exist
```

### Solution
We need to install a working python3.5 on our system and pass it as -p argument. One of the way to do it:
```shell script
sudo add-apt-repository ppa:deadsnakes/ppa
sudo apt-get update
sudo apt-get install python3.5
sudo apt-get install python3.5-dev
```

to make sure that the python3.5 is installed correctly, try the following command: `python3.5`

the following output should appear:
```shell script
Python 3.5.9 (default, Nov 24 2019, 01:35:13)
[GCC 7.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information
>>>
```

Now, you can run the command `virtualenv -p python3.5 venv` again.

## Installation of requirements fails
### Problem
```shell script
pip install -r requirements.txt
.
.
.
src/_portaudiomodule.c:29:10: fatal error: portaudio.h: No such file or directory
     #include "portaudio.h"
              ^~~~~~~~~~~~~
    compilation terminated.
    error: command 'x86_64-linux-gnu-gcc' failed with exit status 1
    ----------------------------------------
ERROR: Command errored out with exit status 1: /home/damian/python_envs/.dictation_client_venv35/bin/python3.5 -u -c 'import sys, setuptools, tokenize; sys.argv[0] = '"'"'/tmp/pip-install-uncn6w85/pyaudio/setup.py'"'"'; __file__='"'"'/tmp/pip-install-uncn6w85/pyaudio/setup.p
y'"'"';f=getattr(tokenize, '"'"'open'"'"', open)(__file__);code=f.read().replace('"'"'\r\n'"'"', '"'"'\n'"'"');f.close();exec(compile(code, __file__, '"'"'exec'"'"'))' install --record /tmp/pip-record-ri2744y6/install-record.txt --single-version-externally-managed --compile 
--install-headers /home/damian/python_envs/.dictation_client_venv35/include/site/python3.5/pyaudio Check the logs for full command output.
```
### Solution
If the installation fails with an error like the one above, we need to install the portaudio package on our machine.
Run `sudo apt-get install portaudio19-dev python-pyaudio python3-pyaudio`
Once this is installed, try running the `pip install -r requirements.txt` again. This time it should install correctly.

## Using the client fails with _Connect failed_ error
### Problem
```shell script
python dictation_client.py --service-address "185.110.50.61:62075" --wave-path ~/tasks/4_12_2040_io/test_data/105_2040_InternalTests_01/Spkr001_bot_questions/105_2040_InternalTests_01_Spkr001_bot_questions_001.wav

/home/damian/python_envs/.dictation_client_venv35/lib/python3.5/site-packages/pydub/utils.py:165: RuntimeWarning: Couldn't find ffmpeg or avconv - defaulting to ffmpeg, but may not work
  warn("Couldn't find ffmpeg or avconv - defaulting to ffmpeg, but may not work", RuntimeWarning)
Dictation ASR gRPC client 2.1.0
Recognizing...
Traceback (most recent call last):
  File "dictation_client.py", line 80, in <module>
    results = recognizer.recognize(stream)
  File "/home/damian/Projects/projects/dictation-client/python/service/streaming_recognizer.py", line 51, in recognize
    return self.recognize_audio_content(requests_iterator)
  File "/home/damian/Projects/projects/dictation-client/python/service/streaming_recognizer.py", line 69, in recognize_audio_content
    for recognition in recognitions:
  File "/home/damian/python_envs/.dictation_client_venv35/lib/python3.5/site-packages/grpc/_channel.py", line 348, in __next__
    return self._next()
  File "/home/damian/python_envs/.dictation_client_venv35/lib/python3.5/site-packages/grpc/_channel.py", line 342, in _next
    raise self
grpc._channel._Rendezvous: <_Rendezvous of RPC that terminated with (StatusCode.UNAVAILABLE, Connect Failed)>
```
### Solution
This error means, that the IP address passed with the _--service-address_ argument is invalid and there is no running 
instance of the server there. Please ask the service provider to supply you with the current address.

# Usage:
```
usage: dictation_client.py [-h] --service-address ADDRESS [--wave-path WAVE]
                           [--mic] [--session-id SESSION_ID]
                           [--grpc-timeout GRPC_TIMEOUT]
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
  --grpc-timeout GRPC_TIMEOUT
                        Timeout in milliseconds used to set gRPC deadline -
                        how long the client is willing to wait for a reply
                        from the server. If not specified, the service will
                        set the deadline to a very large number.
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
  --context-phrase      Specifies which context model to use.
```
