import sounddevice
import queue


class MicrophoneStream(object):
    """Opens a recording stream as a generator yielding the audio chunks."""
    """It's directly copied from GOOGLE example:
    https://cloud.google.com/speech/docs/streaming-recognize#speech-streaming-recognize-python """

    def __init__(self, rate, chunk):
        self._rate = rate
        self._chunk = chunk

        # Create a thread-safe buffer of audio data
        self._buff = queue.Queue()
        self.closed = True

    def __enter__(self):
        self._audio_stream = sounddevice.RawInputStream(
            samplerate=self._rate,
            blocksize=self._chunk,
            # The API currently only supports 1-channel (mono) audio
            # https://goo.gl/z757pE
            channels=1,
            dtype='int16',
            callback=self._fill_buffer
        )

        self._audio_stream.start()
        self.closed = False

        return self

    def __exit__(self, type, value, traceback):
        self._audio_stream.close()
        self.closed = True
        # Signal the generator to terminate so that the client's
        # streaming_recognize method will not block the process termination.
        self._buff.put(None)

    def _fill_buffer(self, in_data, frames, time, status):
        """Continuously collect data from the audio stream, into the buffer."""
        self._buff.put(bytes(in_data))
        return None

    def close(self):
        self.closed = True

    def frame_rate(self):
        return self._rate

    def session_id(self):
        return "sarmata-pyclient-mic"

    def generator(self):
        while not self.closed:
            # Use a blocking get() to ensure there's at least one chunk of
            # data, and stop iteration if the chunk is None, indicating the
            # end of the audio stream.
            chunk = self._buff.get()
            if chunk is None:
                return
            data = [chunk]

            # Now consume whatever other data's still buffered.
            while True:
                try:
                    chunk = self._buff.get(block=False)
                    if chunk is None:
                        return
                    data.append(chunk)
                except queue.Empty:
                    break

            yield b''.join(data)
# [END audio_stream]
