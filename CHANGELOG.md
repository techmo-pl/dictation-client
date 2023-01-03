# Techmo dictation-client repository changelog

This changelog is intended for tracking content changes in whole repository.
We are using single version and changelog for all components.
Dev note: Remember to synchronize version in files: `python/VERSION.py` and `cpp/libdictation-client/VERSION.h`


## [2.8.0] - 2023-01-03

### Changed
 - [python] Drop support for Python 3.6
 - [python] Upgrade PyAudio to 0.2.13
 - [python] Upgrade setuptools to 65.5.1


## [2.7.0] - 2022-12-19

### Added
 - [python] Support for `sync` requests
 - [python] Handling of `--max-alternatives` parameter
### Changed
 - [CPP, python, tools] Switch GRPC version to 1.43.0


## [2.6.0] - 2022-10-03

### Added
 - [python] `--frame-length` parameter
### Changed
 - [CPP, python] Improve googleapis build script.
 - [CPP] Improve proto build script.
 - [CPP, python] Unify docker image builds.


## [2.5.0] - 2022-07-13

### Added
 - [CPP, python] `--wait-for-service-start` parameter


## [2.4.1] - 2022-06-29

### Changed
 - [CPP, python] `ssl-dir` option renamed to `tls-dir`
 - [CPP] Use googleapis and protobuf from `/opt/grpc_v1.38.1` instead of submodules


## [2.4.0] - 2021-12-17

### Added
 - [CPP, python] Ogg Vorbis files handling
### Changed
 - [CPP, python] `wav-path` option renamed to `audio-path`


## [2.3.3] - 2021-08-01

### Changed
 - [CPP] Upgrade Boost version to 1.74.0


## [2.3.2] - 2021-07-21

### Changed
 - [CPP, python, tools] Switch GRPC version to 1.38.1
 - [python] Protobuf version upgraded to 3.12.2


## [2.3.1] - 2020-07-10

### Changed
 - Protobuf version in gRPC upgraded to 3.11.3


## [2.3.0] - 2020-05-12

### Added
 - [CPP, python] - `ssl-dir` option [bool], for enabling ssl communication.


## [2.2.1] - 2020-01-22

### Changed
 - [CPP] - Added realtime processing in streaming mode.
 - [CPP] - Added check if input wav is mono channel.


## [2.2.0] - 2020-01-22

### Changed
 - [CPP, python, tools] Switch GRPC version to 1.24.3.
 - [CPP, python] - Switch to single CHANGELOG.md and consistent version for all components.
### Fixed
 - [python] Change to request iterator to stop on null data.


## [2.1.0] - 2018-12-12

### Added
- [CPP, python] Support for setting gRPC deadline (how long the client is willing to wait for a reply from the server).


## [2.0.1] - 2018-11-21

### Fixed
- [python] Stream is now half-closed after reading an error response.


## [2.0.0] - 2018-09-28

### Added
- [CPP, python] Returning recognition result finalization cause to API.
- [python] Support for speech incomplete timeout setting.
### Removed
- [CPP, python] `NO_INPUT_TIMEOUT` speech event type from API.


## [1.1.0] - 2018-07-06

### Added
- [python] Microphone as an audio source support.


## [1.0.2] - 2018-03-23

### Changed
- [CPP] `DictationSessionConfig` to have fields for sample rate, encoding, language code and to store service settings in a map instead of a string.


## [1.0.1] - 2018-01-31

### Added
- [CPP] gRPC status code names printing on error.
### Changed
- [CPP] Updates in Readmes.
### Fixed
- [CPP] sh to bash in tools scripts.
- [python] Handling multiple response from server (`single-utterance == false`) for single wave file


## [1.0.0] - 2018-01-25

### Added
- [CPP] Initial implementation.
