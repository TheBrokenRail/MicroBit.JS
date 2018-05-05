#!/usr/bin/env bash

set -e

tput setaf 3; tput bold; echo "Generating C++ Source for JS File"
printf '#include <string>\nchar *jsSource = (char *)R"~~~~('"$(cat Source/init.js)"'\n'$(cat Source/main.js)""')~~~~";' > Source/JSSource.h

tput setaf 3; tput bold; echo "Cloning mJS"
git clone --depth=1 https://github.com/cesanta/mjs.git
cp mjs/mjs.c Source
cp mjs/mjs.h Source

mkdir Lib
git clone https://github.com/jerryscript-project/jerryscript.git
cd jerryscript
mkdir build
EXT_CFLAGS='-D__TARGET_MBED_BBC-MICROBIT-CLASSIC-GCC'
export CC=/usr/bin/arm-none-eabi-gcc
cmake -Bbuild -H./ \
  -DCMAKE_SYSTEM_NAME=mbedOS \
  -DCMAKE_SYSTEM_PROCESSOR=armv7-m \
  #-DCMAKE_C_COMPILER_WORKS=TRUE \
  -DENABLE_LTO=OFF \
  -DENABLE_ALL_IN_ONE=OFF \
  -DJERRY_LIBC=OFF \
  -DJERRY_CMDLINE=OFF \
  -DEXTERNAL_COMPILE_FLAGS="$EXT_CFLAGS" \
  -DMEM_HEAP_SIZE_KB=16
make -Cbuild jerry-core
make -Cbuild jerry-libm
dir build
dir build/lib
cp -r build/lib/. ../Lib
cd ../
dir Lib

tput setaf 3; tput bold; echo "Installing Yotta"
pip install --user yotta
tput setaf 3; tput bold; echo "Setting Yotta Target"
yotta target bbc-microbit-classic-gcc
tput setaf 3; tput bold; echo "Building Project"
VERBOSE=1 yotta build
tput setaf 3; tput bold; echo "Copying Output"
cp build/bbc-microbit-classic-gcc/Source/microbit-js-combined.hex GH-Pages/microbit-js.hex 
