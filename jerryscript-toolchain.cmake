include("${CMAKE_CURRENT_LIST_DIR}/yotta_targets/mbed-gcc/CMake/toolchain.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/yotta_targets/bbc-microbit-classic-gcc/CMake/toolchain.cmake")
set(EXTERNAL_COMPILE_FLAGS "${EXTERNAL_COMPILE_FLAGS} -Wno-error=implicit-fallthrough")
