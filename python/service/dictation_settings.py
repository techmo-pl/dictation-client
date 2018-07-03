class DictationSettings:
    """Default settings for Techmo Dictation ASR (timeouts and thresholds)"""

    def __init__(self, args):
        # use configuration directly
        self.args = args

    def session_id(self):
        return self.args.session_id

    def max_alternatives(self):
        return self.args.max_alternatives

    def time_offsets(self):
        return self.args.time_offsets

    def single_utterance(self):
        return self.args.single_utterance

    def interim_results(self):
        return self.args.interim_results

    def timeouts_map(self):

        # timeouts
        rec_timeout = (1.0 * self.args.recognition_timeout) / 1000.0
        comp_timeout = (1.0 * self.args.speech_complete_timeout) / 1000.0

        timeouts = {
            "no-input-timeout": str(self.args.no_input_timeout),
            "endpoint.rule5.min-utterance-length": str(rec_timeout),
            "endpoint.rule2.min-trailing-silence": str(comp_timeout),
            "endpoint.rule3.min-trailing-silence": str(comp_timeout),
            "endpoint.rule4.min-trailing-silence": str(comp_timeout),
        }

        return timeouts
