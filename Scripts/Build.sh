#!/usr/bin/env bash

set -e

echo "Generating C++ Source for JS File"
printf '#include <string>\nstd::string jsSource = R"~~~~('"$(cat Source/main.js)"')~~~~";' > Source/JSSource.h

mkdir Build
mkdir Source/include

echo "Cloning mJS"
git clone --depth=1 https://github.com/cesanta/mjs.git
cp mjs/mjs.c Source/include
cp mjs/mjs.h Source/include

echo "Installing Yotta"
pip install --user yotta
echo "Setting Yotta Target"
yotta target bbc-microbit-classic-gcc
echo "Building Project"
yotta build
echo "Copying Output to GH-Pages"
cp build/bbc-microbit-classic-gcc/Source/MicroBit-JS-combined.hex Build
