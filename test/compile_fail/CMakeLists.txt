#-----------------------------------------------------------------------------
#
#  CMake config
#
#  protozero compile failure tests
#
#  Each source file in here must fail to compile with the expected error
#  message. This is used to test that misuse of the API is caught at compile
#  time, for instance by a static_assert.
#
#  To add a test, create a new source file and add its name (without the
#  .cpp suffix) to COMPILE_FAIL_TESTS below.
#
#-----------------------------------------------------------------------------

set(COMPILE_FAIL_TESTS builder_parent_wrong_enum
                       builder_parent_integer_tag)

foreach(_test IN LISTS COMPILE_FAIL_TESTS)
    # Compile-only target (no main() needed), excluded from the normal build.
    add_library(compile_fail_${_test} OBJECT EXCLUDE_FROM_ALL ${_test}.cpp)
    set_target_properties(compile_fail_${_test} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)

    # The test builds the target above, so its output is the compiler output.
    add_test(NAME compile_fail_${_test}
             COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                                      --target compile_fail_${_test}
                                      --config $<CONFIG>)

    # Pass only if the expected error shows up (the exit code is ignored).
    # Unlike WILL_FAIL, this doesn't accept unrelated compile errors.
    set_tests_properties(compile_fail_${_test} PROPERTIES
                         PASS_REGULAR_EXPRESSION "tag must be of the parent builder's enum type")
endforeach()


#-----------------------------------------------------------------------------
