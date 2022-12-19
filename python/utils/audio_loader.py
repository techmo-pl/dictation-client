import warnings
from pathlib import Path
import pydub


# WAVs don’t require ffmpeg; it's ok to ignore the warning.
warnings.filterwarnings(
    "ignore",
    module="pydub",
    category=RuntimeWarning,
    message="Couldn't find ffmpeg or avconv - defaulting to ffmpeg, but may not work",
)

class AudioLoadingError(RuntimeError):
    def __init__(self, audio_file_path: Path, message: str):
        super().__init__(
            f"{audio_file_path} {message}: only mono, 16 bit PCM audio files are allowed"
        )


class AudioLoader:
    def __init__(self, audio_file_path: Path):
        try:
            self._audio_segment = pydub.AudioSegment.from_wav(audio_file_path)
        except pydub.exceptions.CouldntDecodeError:
            raise AudioLoadingError(audio_file_path, "has an unknown format")

        if self._audio_segment.channels != 1:
            raise AudioLoadingError(
                audio_file_path, f"has {self._audio_segment.channels} channels"
            )

        self._bit_depth = self._audio_segment.sample_width * 8

        if self._bit_depth != 16:
            raise AudioLoadingError(audio_file_path, f"has {self._bit_depth} bit depth")

        self._sampling_rate_hz = self._audio_segment.frame_rate

        self._audio_data = self._audio_segment.raw_data

    @property
    def bit_depth(self):
        return self._bit_depth

    @property
    def sampling_rate_hz(self):
        return self._sampling_rate_hz

    @property
    def audio_data(self):
        return self._audio_data
