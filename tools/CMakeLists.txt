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
endif()


#-----------------------------------------------------------------------------
