{
  "includes": [
      "common.gypi"
  ],
  "targets": [
    {
      "target_name": "reader_tests",
      "type": "executable",
      "sources": [
        "test/reader_tests.cpp",
        "test/t/basic/reader_test_cases.cpp",
        "test/t/bool/reader_test_cases.cpp",
        "test/t/bytes/reader_test_cases.cpp",
        "test/t/complex/reader_test_cases.cpp",
        "test/t/double/reader_test_cases.cpp",
        "test/t/enum/reader_test_cases.cpp",
        "test/t/fixed32/reader_test_cases.cpp",
        "test/t/fixed64/reader_test_cases.cpp",
        "test/t/float/reader_test_cases.cpp",
        "test/t/int32/reader_test_cases.cpp",
        "test/t/int64/reader_test_cases.cpp",
        "test/t/message/reader_test_cases.cpp",
        "test/t/repeated/reader_test_cases.cpp",
        "test/t/repeated_packed_fixed32/reader_test_cases.cpp",
        "test/t/repeated_packed_fixed64/reader_test_cases.cpp",
        "test/t/repeated_packed_int32/reader_test_cases.cpp",
        "test/t/repeated_packed_int64/reader_test_cases.cpp",
        "test/t/repeated_packed_sfixed32/reader_test_cases.cpp",
        "test/t/repeated_packed_sfixed64/reader_test_cases.cpp",
        "test/t/repeated_packed_sint32/reader_test_cases.cpp",
        "test/t/repeated_packed_sint64/reader_test_cases.cpp",
        "test/t/repeated_packed_uint32/reader_test_cases.cpp",
        "test/t/repeated_packed_uint64/reader_test_cases.cpp",
        "test/t/sfixed32/reader_test_cases.cpp",
        "test/t/sfixed64/reader_test_cases.cpp",
        "test/t/sint32/reader_test_cases.cpp",
        "test/t/sint64/reader_test_cases.cpp",
        "test/t/skip/reader_test_cases.cpp",
        "test/t/string/reader_test_cases.cpp",
        "test/t/tags/reader_test_cases.cpp",
        "test/t/uint32/reader_test_cases.cpp",
        "test/t/uint64/reader_test_cases.cpp",
        "test/t/vector_tile/reader_test_cases.cpp",
        "test/t/zigzag/reader_test_cases.cpp"
      ],
      "include_dirs": [
          "./include/",
          "./test/include/"
      ]
    }
  ]
}
