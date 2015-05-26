{
  "includes": [
      "common.gypi"
  ],
  "targets": [
    {
      "target_name": "tests",
      "type": "executable",
      "sources": [
        "test/run_all_tests.cpp",
        "test/t/basic/runtest.cpp",
        "test/t/bool/runtest.cpp",
        "test/t/bytes/runtest.cpp",
        "test/t/complex/runtest.cpp",
        "test/t/double/runtest.cpp",
        "test/t/enum/runtest.cpp",
        "test/t/fixed32/runtest.cpp",
        "test/t/fixed64/runtest.cpp",
        "test/t/float/runtest.cpp",
        "test/t/int32/runtest.cpp",
        "test/t/int64/runtest.cpp",
        "test/t/message/runtest.cpp",
        "test/t/repeated/runtest.cpp",
        "test/t/repeated_packed_fixed32/runtest.cpp",
        "test/t/repeated_packed_fixed64/runtest.cpp",
        "test/t/repeated_packed_int32/runtest.cpp",
        "test/t/repeated_packed_int64/runtest.cpp",
        "test/t/repeated_packed_sfixed32/runtest.cpp",
        "test/t/repeated_packed_sfixed64/runtest.cpp",
        "test/t/repeated_packed_sint32/runtest.cpp",
        "test/t/repeated_packed_sint64/runtest.cpp",
        "test/t/repeated_packed_uint32/runtest.cpp",
        "test/t/repeated_packed_uint64/runtest.cpp",
        "test/t/sfixed32/runtest.cpp",
        "test/t/sfixed64/runtest.cpp",
        "test/t/sint32/runtest.cpp",
        "test/t/sint64/runtest.cpp",
        "test/t/skip/runtest.cpp",
        "test/t/string/runtest.cpp",
        "test/t/tags/runtest.cpp",
        "test/t/uint32/runtest.cpp",
        "test/t/uint64/runtest.cpp",
        "test/t/vector_tile/runtest.cpp",
        "test/t/zigzag/runtest.cpp"
      ],
      "xcode_settings": {
        "SDKROOT": "macosx",
        "SUPPORTED_PLATFORMS":["macosx"]
      },
      "include_dirs": [
          "./"
      ]
    }
  ]
}
