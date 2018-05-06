include("${CMAKE_CURRENT_LIST_DIR}/yotta_targets/mbed-gcc/CMake/toolchain.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/yotta_targets/bbc-microbit-classic-gcc/CMake/toolchain.cmake")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-fallthrough")
