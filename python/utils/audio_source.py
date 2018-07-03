from pydub import AudioSegment
import os


class AudioStream(object):

    def __init__(self, audio_path):
        if not os.path.exists(audio_path):
            raise ValueError("Wave file does not exist at: {}".format(audio_path))

        self.audio_segment = AudioSegment.from_wav(audio_path)
        self.audio_path = audio_path

        if self.audio_segment.channels != 1:
            raise ValueError("Only mono waves are allowed. {} contains: {} channels".format(audio_path, self.audio_segment.channels))
        if self.audio_segment.sample_width != 2:
            raise ValueError(
                "Only 16bit samples are allowed. {} has: {} bit samples".format(audio_path, self.audio_segment.sample_width * 8))

        self.data_index = 0
        self.audio = self.audio_segment.raw_data

        frame_len = 200  # ms
        sample_width = 2  # 16bit
        self.frame_samples_size = (self.audio_segment.frame_rate // 1000) * frame_len * sample_width

    def __iter__(self):
        return self

    def __next__(self):
        if self.data_index >= len(self.audio):
            raise StopIteration()

        end_sample = self.data_index + self.frame_samples_size
        if end_sample >= len(self.audio):
            end_sample = len(self.audio)

        data = self.audio[self.data_index: end_sample]
        self.data_index = end_sample

        return data

    def __enter__(self):
        self.data_index = 0
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        pass

    def frame_rate(self):
        return self.audio_segment.frame_rate

    def session_id(self):
        return os.path.basename(self.audio_path)

    def close(self):
        self.data_index = len(self.audio) + 1  # mark end-of-stream

    def generator(self):
        while self.data_index < len(self.audio):
            end_sample = self.data_index + self.frame_samples_size
            if end_sample >= len(self.audio):
                end_sample = len(self.audio)

            data = self.audio[self.data_index: end_sample]
            self.data_index = end_sample
            yield data
