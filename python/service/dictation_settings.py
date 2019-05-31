class DictationSettings:
    """Default settings for Techmo Dictation ASR (timeouts and thresholds)"""

    def __init__(self, args):
        # use configuration directly
        self.args = args

    def session_id(self):
        return self.args.session_id

    def grpc_timeout(self):
        return self.args.grpc_timeout

    def max_alternatives(self):
        return self.args.max_alternatives

    def time_offsets(self):
        return self.args.time_offsets

    def single_utterance(self):
        return self.args.single_utterance

    def interim_results(self):
        return self.args.interim_results

    def timeouts_map(self):
        return {
            "no-input-timeout": str(self.args.no_input_timeout),
            "speech-complete-timeout": str(self.args.speech_complete_timeout),
            "speech-incomplete-timeout": str(self.args.speech_incomplete_timeout),
            "recognition-timeout": str(self.args.recognition_timeout),
        }

    def context_phrase(self):
        return self.args.context_phrase
