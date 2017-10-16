#-----------------------------------------------------------------------------
#
#  CMake config
#
#  protozero tools
#
#-----------------------------------------------------------------------------

# Needs getopt, which is not available on Windows
if(NOT MSVC)
    add_executable(pbf-decoder pbf-decoder.cpp)

    add_test(NAME pbf-decoder-no-args
             COMMAND pbf-decoder)
    set_tests_properties(pbf-decoder-no-args PROPERTIES
                         PASS_REGULAR_EXPRESSION "^Usage:")

    add_test(NAME pbf-decoder-help
             COMMAND pbf-decoder --help)
    set_tests_properties(pbf-decoder-help PROPERTIES
                         PASS_REGULAR_EXPRESSION "^Usage:")

    add_test(NAME pbf-decoder-empty
             COMMAND pbf-decoder "${CMAKE_SOURCE_DIR}/test/t/message/data-opt-empty.pbf")
    set_tests_properties(pbf-decoder-empty PROPERTIES
                         PASS_REGULAR_EXPRESSION "^$")

    add_test(NAME pbf-decoder-data
             COMMAND pbf-decoder "${CMAKE_SOURCE_DIR}/test/t/message/data-message.pbf")
    set_tests_properties(pbf-decoder-data PROPERTIES
                         PASS_REGULAR_EXPRESSION "^1:")

    add_test(NAME pbf-decoder-vt
             COMMAND pbf-decoder -l 999999 -o 0 "${CMAKE_SOURCE_DIR}/test/t/vector_tile/data.vector.pbf")
    set_tests_properties(pbf-decoder-vt PROPERTIES
                         PASS_REGULAR_EXPRESSION "^3:")

endif()


#-----------------------------------------------------------------------------
