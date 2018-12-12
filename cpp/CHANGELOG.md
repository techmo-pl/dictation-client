# Dictation ASR gRPC C++ client Changelog

## [2.1.0] - 2018-12-12
### Added
- Support for setting gRPC deadline (how long the client is willing to wait for a reply from the server).

## [2.0.0] - 2018-09-28
### Added
- Returning recognition result finalization cause to API.

### Removed
- `NO_INPUT_TIMEOUT` speech event type from API.

## [1.0.2] - 2018-03-23
### Changed
- `DictationSessionConfig` to have fields for sample rate, encoding, language code and to store service settings in a map instead of a string.

## [1.0.1] - 2018-01-31
### Added
- gRPC status code names printing on error.

### Changed
- Updates in Readmes.

### Fixed
- sh to bash in tools scripts.

## [1.0.0] - 2018-01-25
### Added
- Initial implementation.
