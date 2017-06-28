
# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [unreleased] -

### Added

- Add missing two-parameter version of `pbf_message::next()` function.

### Changed

- Clarify include file usage in tutorial.
- Updated included Catch unit test framework to version 1.9.6 and updated
  tests to work with the current version.

### Fixed

- Important bugfix in `data_view` equality operator. The equality operator is
  actually never used in the protozero code itself, but users of protozero
  might use it. This is a serious bug that could lead to buffer overrun type
  problems.


## [1.5.1] - 2017-01-14

### Added

- Better documentation for `tag_and_type()` in doc/advanced.md.

### Fixed

- Fixed broken "make doc" build.


## [1.5.0] - 2017-01-12

### Added

- Add `add_bytes_vectored()` methods to `pbf_writer` and `pbf_builder`. This
  allows single-copy scatter-gather type adding of data that has been prepared
  in pieces to a protobuf message.
- New functions to check the tag and wire type at the same time: Two parameter
  version of `pbf_reader::next()` and `pbf_reader::tag_and_type()` can be used
  together with the free function `tag_and_type()` to easily and quickly check
  that not only the tag but also the wire type is correct for a field.

### Changed

- `packed_field_*` classes now work with `pbf_builder`.
- Reorganized documentation. Advanced docs are now under doc/advanced.md.

### Fixed

- `packed_field` class is now non-copyable because data can get corrupted if
  you copy it around.
- Comparison operators of `data_view` now have const& parameters.
- Make zigzag encoding/decoding functions constexpr.


## [1.4.5] - 2016-11-18

### Fixed

- Undefined behaviour in packed fixed iterator. As a result, the macro
  `PROTOZERO_DO_NOT_USE_BARE_POINTER` is not used any more.


## [1.4.4] - 2016-11-15

### Fixed

- Byteswap implementation.


## [1.4.3] - 2016-11-15

### Fixed

- Undefined behaviour in byte swapping code.
- Rename some parameters to avoid "shadow" warning from some compilers.


## [1.4.2] - 2016-08-27

### Fixed

- Compile fix: Variable shadowing.


## [1.4.1] - 2016-08-21

### Fixed

- GCC 4.8 compile fixed

### Added

- New ability to dynamically require the module as a node module to ease
  building against from other node C++ modules.

## [1.4.0] - 2016-07-22

### Changed

- Use more efficient new `skip_varint()` function when iterating over
  packed varints.
- Split `decode_varint()` function into two functions speeding up the
  common case where a varint is only one byte long.
- Introduce new class `iterator_range` used instead of `std::pair` of
  iterators. This way the objects can be used in range-based for loops.
  Read UPGRADING.md for details.
- Introduce new class `data_view` and functions using and returning it.
  Read UPGRADING.md for details.


## [1.3.0] - 2016-02-18

### Added

- Added `config.hpp` header which now includes all the macro magic to
  configure the library for different architectures etc.
- New way to create repeated packed fields without using an iterator.
- Add `rollback()` function to `pbf_writer` for "manual" rollback.

### Changed

- Various test and documentation cleanups.
- Rename `pbf_types.hpp` to `types.hpp`.


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


[unreleased]: https://github.com/osmcode/libosmium/compare/v1.5.1...HEAD
[1.5.1]: https://github.com/osmcode/libosmium/compare/v1.5.0...v1.5.1
[1.5.0]: https://github.com/osmcode/libosmium/compare/v1.4.5...v1.5.0
[1.4.5]: https://github.com/osmcode/libosmium/compare/v1.4.4...v1.4.5
[1.4.4]: https://github.com/osmcode/libosmium/compare/v1.4.3...v1.4.4
[1.4.3]: https://github.com/osmcode/libosmium/compare/v1.4.2...v1.4.3
[1.4.2]: https://github.com/osmcode/libosmium/compare/v1.4.1...v1.4.2
[1.4.1]: https://github.com/osmcode/libosmium/compare/v1.4.0...v1.4.1
[1.4.0]: https://github.com/osmcode/libosmium/compare/v1.3.0...v1.4.0
[1.3.0]: https://github.com/osmcode/libosmium/compare/v1.2.3...v1.3.0
[1.2.3]: https://github.com/osmcode/libosmium/compare/v1.2.2...v1.2.3
[1.2.2]: https://github.com/osmcode/libosmium/compare/v1.2.1...v1.2.2
[1.2.1]: https://github.com/osmcode/libosmium/compare/v1.2.0...v1.2.1
[1.2.0]: https://github.com/osmcode/libosmium/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/osmcode/libosmium/compare/v1.0.0...v1.1.0

