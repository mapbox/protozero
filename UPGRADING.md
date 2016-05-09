
# Upgrading

This file contains instructions for users of Protozero who are upgrading from
one version to another.

You do not need to change anything if only the minor version changes, but it
is better to keep up with changes if you can. The switch to the next major
version will be easier then. And you might get some more convenient usage.

To help you with upgrading to new versions, you can define the C++ preprocessor
macro `PROTOZERO_STRICT_API` in which case Protozero will compile without the
code used for backwards compatibilty. You will then get compile errors for
older API usages.

## Upgrading from *v1.3.0* to *master*

* Functions in `pbf_reader` (and the derived `pbf_message`) called
  `get_packed_*()` now return an `iterator_range` instead of a `std::pair`.
  The new class is derived from `std::pair`, so changes are usually not
  strictly necessary. For future compatibility, you should change all
  attribute accesses on the returned objects from `first` and `second` to
  `begin()` and `end()`, respectively. So change something like this:

    auto x = message.get_packed_int32();
    for (auto it = x.first; it != x.second; ++it) {
        ....
    }
 
  to: 

    auto x = message.get_packed_int32();
    for (auto it = x.begin(); it != x.end(); ++it) {
        ....
    }

  or even better use the range-based for loop:

    auto x = message.get_packed_int32();
    for (auto val : x) {
        ....
    }

