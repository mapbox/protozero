
# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [unreleased] -

### Added

- pbf_message and pbf_builder template classes wrapping pbf_reader
  and pbf_writer, respectively. The new classes are the preferred
  interface now.

### Changed

- Improved byte swapping operation.
- Detect some types of data corruption earlier and throw.

### Fixed


## [1.1.0] - 2015-08-22

### Changed

- Make pbf reader and writer code endianess-aware.


[unreleased]: https://github.com/osmcode/libosmium/compare/v1.1.0...HEAD
[1.1.0]: https://github.com/osmcode/libosmium/compare/v1.0.0...v1.1.0

