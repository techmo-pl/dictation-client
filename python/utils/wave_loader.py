from pydub import AudioSegment
import os


def load_wave(path):

    if not os.path.exists(path):
        raise ValueError("Wave file does not exist at: {}".format(path))

    audio = AudioSegment.from_wav(path)

    if audio.channels != 1:
        raise ValueError("Only mono waves are allowed. {} contains: {} channels".format(path, audio.channels))
    if audio.sample_width != 2:
        raise ValueError("Only 16bit samples are allowed. {} has: {} bit samples".format(path, audio.sample_width * 8))

    return {"samples": audio.raw_data, "frame_rate": audio.frame_rate}
