#!/usr/bin/env bash

set -e

mkdir Build
mkdir Source/include

wget http://dev.mujs.com/downloads/mujs-1.0.3.tar.xz
tar xvfJ http://dev.mujs.com/downloads/mujs-1.0.3.tar.xz
cd mujs-1.0.3
make
dir build
cd ../

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
echo $CPPFLAGS
export CPPFLAGS="-fdata-sections -ffunction-sections -Wl,--gc-sections"
yotta build
cp build/bbc-microbit-classic-gcc/Source/MicroBit.JS-combined.hex Build
