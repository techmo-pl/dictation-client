# Dictation ASR gRPC Python client Changelog

## [2.1.0] - 2018-12-12
### Added
- Support for setting gRPC deadline (how long the client is willing to wait for a reply from the server).

## [2.0.1] - 2018-11-21
### Fixed
- Stream is now half-closed after reading an error response.

## [2.0.0] - 2018-09-28
### Added
- Returning recognition result finalization cause to API.
- Support for speech incomplete timeout setting.

### Removed
- `NO_INPUT_TIMEOUT` speech event type from API.

## [1.1.0] - 2018-07-06
### Added
- Microphone as an audio source support.

## [1.0.1] - 2018-06-28
### Fixed
- Handling multiple response from server (`single-utterance == false`) for single wave file

## [1.0.0] - 2018-02-23
### Added
- Initial implementation.
