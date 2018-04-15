#!/usr/bin/env bash

set -e

mkdir Build
mkdir Source/include

wget http://duktape.org/duktape-2.2.0.tar.xz
tar xvfJ duktape-2.2.0.tar.xz
cd duktape-2.2.0
rm -rf src-custom
python tools/configure.py --output-directory ../Source/include
cd ../

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
echo $CPPFLAGS
export CPPFLAGS="-fdata-sections -ffunction-sections -Wl,--gc-sections"
yotta build
cp build/bbc-microbit-classic-gcc/Source/MicroBit.JS-combined.hex Build
