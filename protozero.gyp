{
  "includes": [
      "common.gypi"
  ],
  "targets": [
    {
      "target_name": "tests",
      "type": "executable",
      "sources": [
        "test/tests.cpp",
        "test/t/basic/test_cases.cpp",
        "test/t/bool/test_cases.cpp",
        "test/t/bytes/test_cases.cpp",
        "test/t/complex/test_cases.cpp",
        "test/t/double/test_cases.cpp",
        "test/t/endian/test_cases.cpp",
        "test/t/enum/test_cases.cpp",
        "test/t/exceptions/test_cases.cpp",
        "test/t/fixed32/test_cases.cpp",
        "test/t/fixed64/test_cases.cpp",
        "test/t/float/test_cases.cpp",
        "test/t/int32/test_cases.cpp",
        "test/t/int64/test_cases.cpp",
        "test/t/message/test_cases.cpp",
        "test/t/nested/test_cases.cpp",
        "test/t/repeated_packed_bool/test_cases.cpp",
        "test/t/repeated_packed_double/test_cases.cpp",
        "test/t/repeated_packed_enum/test_cases.cpp",
        "test/t/repeated_packed_fixed32/test_cases.cpp",
        "test/t/repeated_packed_fixed64/test_cases.cpp",
        "test/t/repeated_packed_float/test_cases.cpp",
        "test/t/repeated_packed_int32/test_cases.cpp",
        "test/t/repeated_packed_int64/test_cases.cpp",
        "test/t/repeated_packed_sfixed32/test_cases.cpp",
        "test/t/repeated_packed_sfixed64/test_cases.cpp",
        "test/t/repeated_packed_sint32/test_cases.cpp",
        "test/t/repeated_packed_sint64/test_cases.cpp",
        "test/t/repeated_packed_uint32/test_cases.cpp",
        "test/t/repeated_packed_uint64/test_cases.cpp",
        "test/t/repeated/test_cases.cpp",
        "test/t/sfixed32/test_cases.cpp",
        "test/t/sfixed64/test_cases.cpp",
        "test/t/sint32/test_cases.cpp",
        "test/t/sint64/test_cases.cpp",
        "test/t/skip/test_cases.cpp",
        "test/t/string/test_cases.cpp",
        "test/t/tags/test_cases.cpp",
        "test/t/uint32/test_cases.cpp",
        "test/t/uint64/test_cases.cpp",
        "test/t/varint/test_cases.cpp",
        "test/t/vector_tile/test_cases.cpp",
        "test/t/zigzag/test_cases.cpp"
      ],
      "include_dirs": [
        "./include/",
        "./test/include/"
      ]
    }
  ]
}
