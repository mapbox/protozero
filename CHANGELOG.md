
# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [unreleased] -

### Added

- Added `config.hpp` header which now includes all the macro magic to
  configure the library for different architectures etc.
- New way to create repeated packed fields without using an iterator.
- Add `rollback()` function to `pbf_writer` for "manual" rollback.

### Changed

- Various test and documentation cleanups.
- Rename `pbf_types.hpp` to `types.hpp`.

### Fixed


## [1.2.3] - 2015-11-30

### Added

- Added `config.hpp` header which now includes all the macro magic to
  configure the library for different architectures etc.

### Fixed

- Unaligned access to floats/doubles on some ARM architectures.


## [1.2.2] - 2015-10-13

### Fixed

- Fix the recently broken writing of bools on big-endian architectures.


## [1.2.1] - 2015-10-12

### Fixed

- Removed unneeded code (1-byte "swap") which lead to test failures.


## [1.2.0] - 2015-10-08

### Added

- `pbf_message` and `pbf_builder` template classes wrapping `pbf_reader`
  and `pbf_writer`, respectively. The new classes are the preferred
  interface now.

### Changed

- Improved byte swapping operation.
- Detect some types of data corruption earlier and throw.


## [1.1.0] - 2015-08-22

### Changed

- Make pbf reader and writer code endianess-aware.


[unreleased]: https://github.com/osmcode/libosmium/compare/v1.2.3...HEAD
[1.2.3]: https://github.com/osmcode/libosmium/compare/v1.2.2...v1.2.3
[1.2.2]: https://github.com/osmcode/libosmium/compare/v1.2.1...v1.2.2
[1.2.1]: https://github.com/osmcode/libosmium/compare/v1.2.0...v1.2.1
[1.2.0]: https://github.com/osmcode/libosmium/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/osmcode/libosmium/compare/v1.0.0...v1.1.0

