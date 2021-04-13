# Python implementation of Dictation ASR gRPC client.


## Basic Usage

Dictation client includes scripts for automatic environment configuration and launching on systems from the Debian Linux family. For launching Dictation client on other Linux-based OS or Windows, check out "Manual Usage" section.


### Before run

To check required dependencies and prepare virtual environment, run:
```
./setup.sh
```

### Run

To run Dictation client, use `run.sh` script, eg:
```
./run --service-address IP_ADDRESS:PORT --wave-path INPUT_WAVE
```
To print usage description, use:
```
./run --help
```


## Manual Usage

### Before run

#### Submodules

After cloning git repository download submodules:
```
git submodule update --init
```
(this command has to be invoked from the project's root directory)

If you are not using git, you have to manually download `googleapis` submodule. 
To do this, open project repository in web browser, go to the `submodules` directory and use the link located there to open the relevant commit in the googleapis repository. Then download it, unpack and copy all files to the `submodules/googleapis` directory.


#### Dependencies

If you don't have virtualenv yet, install it first (https://virtualenv.pypa.io/en/stable/installation.html)
To install virtualenv on Ubuntu is enough to use command:

```
sudo apt-get install virtualenv
```

Then install the required dependencies inside the virtual environment (this step only needs to be done the first time, for the further usage it is enough to use the existing virtual environment).


- On Linux:

Use Python 3 with virtual environment and install required packages (supported Python versions are: 3.5, 3.6, 3.7, 3.8, 3.9):

```
virtualenv -p python3 venv
source venv/bin/activate
pip install -r requirements.txt
```

- On Windows 10:

Temporarily change the PowerShell execution policy to allow scripting. Start PowerShell with `Run as Administrator` and use command:

```
Set-ExecutionPolicy RemoteSigned
```
then confirm your choice.

Use Python 3.6 with virtual environment and install required packages (you can use a different version of Python, but for newer versions it may require building some dependencies manually):

```
virtualenv -p python3.6 venv
.\venv\Scripts\activate
pip install -r requirements.txt
```

To switch back PowerShell execution policy to the default, use command:

```
Set-ExecutionPolicy Restricted
```

#### Proto sources

[Optional] To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

 

### Run

To run Dictation client, activate virtual environment first:
- On Linux:
```
source venv/bin/activate
```
- On Windows:
```
.\venv\Scripts\activate
```
Then run Dictation client. Sample use:

```
python dictation_client.py --service-address "192.168.1.1:4321" --wave-path audio.wav
```

For each request you have to provide the service address and the audio source (wav file or microphone).


### Usage:
```
usage: dictation_client.py [-h] --service-address ADDRESS
                           [--ssl-dir SSL_DIRECTORY] [--wave-path WAVE]
                           [--mic] [--session-id SESSION_ID]
                           [--grpc-timeout GRPC_TIMEOUT]
                           [--max-alternatives MAX_ALTERNATIVES]
                           [--time-offsets] [--single-utterance]
                           [--interim-results]
                           [--no-input-timeout NO_INPUT_TIMEOUT]
                           [--speech-complete-timeout SPEECH_COMPLETE_TIMEOUT]
                           [--speech-incomplete-timeout SPEECH_INCOMPLETE_TIMEOUT]
                           [--recognition-timeout RECOGNITION_TIMEOUT]
                           [--context-phrase CONTEXT_PHRASE]
```

optional arguments:
```
  -h, --help            show this help message and exit
  --service-address ADDRESS
                        IP address and port (address:port) of a service the
                        client will connect to.
  --ssl-dir SSL_DIRECTORY
                        If set to a path with ssl credential files
                        (client.crt, client.key, ca.crt), use ssl
                        authentication. Otherwise use insecure channel
                        (default).
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
  --context-phrase CONTEXT_PHRASE
                        Specifies which context model to use.
```


### Troubleshooting


#### Dependencies

If process of installing dependencies fails with message similar to this one:

```
        src/_portaudiomodule.c:28:10: fatal error: Python.h: No such file or directory
             #include "Python.h"
                      ^~~~~~~~~~
            compilation terminated.
            error: command '/usr/bin/x86_64-linux-gnu-gcc' failed with exit code 1
```
it means that python3-dev package is missing.

On Ubuntu OS you can use:
```
sudo apt-get install python3-dev
```


If process of installing dependencies fails with message similar to this one:

```
        src/_portaudiomodule.c:29:10: fatal error: portaudio.h: No such file or directory
             #include "portaudio.h"
                      ^~~~~~~~~~~~~
            compilation terminated.
            error: command 'x86_64-linux-gnu-gcc' failed with exit status 1
```
it means that PortAudio library is missing.
PortAudio can be downloaded from: http://www.portaudio.com/download.html.
On Ubuntu you can install portaudio using following command:
```
sudo apt-get install -y portaudio19-dev
```


#### Microphone

To use microphone as audio source instead of wav file, use `--mic` option.
It allows to send audio data directly from microphone, however it does not provide information when to finish the recognition.
For this reason in most cases `--mic` should be followed by `--single-utterance` option, which stops recognition after first spotted utterance.

If the only output you receive is:
```
Received speech event type: END_OF_SINGLE_UTTERANCE
```
check that your microphone is connected and properly configured.


#### ALSA Configuration

On Linux operating systems using Advanced Linux Sound Architecture (ALSA) minor configuration changes may be necessary before first use.

If you get following output after runing request:
```
Dictation ASR gRPC client 2.3.0
ALSA lib pcm_dsnoop.c:618:(snd_pcm_dsnoop_open) unable to open slave
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.rear
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.center_lfe
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.side
ALSA lib pcm_route.c:867:(find_matching_chmap) Found no matching channel map
ALSA lib pcm_route.c:867:(find_matching_chmap) Found no matching channel map
ALSA lib pcm_route.c:867:(find_matching_chmap) Found no matching channel map
```
that means you need to modify audio interfaces configuration.

In such case, open `/usr/share/alsa/alsa.conf` file with root privileges, e.g.:
```
sudo nano /usr/share/alsa/alsa.conf
```

In `#  PCM interface` section find and comment (using #) 
all lines that defines interfaces marked as 'Unknown':

```
pcm.rear cards.pcm.rear
pcm.center_lfe cards.pcm.center_lfe
pcm.side cards.pcm.side
```
To get rid of warnings, comment also several lines below, starting with `pcm.surround`.

Then save and close file.


#### FFmpeg

If FFmpeg framework is not installed, the following warning appears in the program output:

```
RuntimeWarning: Couldn't find ffmpeg or avconv - defaulting to ffmpeg, but may not work
  warn("Couldn't find ffmpeg or avconv - defaulting to ffmpeg, but may not work", RuntimeWarning)
```
Installing FFmpeg framework is not necessary to run the application, however it may be useful stuff for everyone working with sound files.

FFmpeg can be downloaded from https://ffmpeg.org/download.html

On Ubuntu/Debian Linux OS you can download and install FFmpeg directly from official repositories.




