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

git clone --depth=1 https://github.com/Aloshi/dukglue.git
cp -r dukglue/include/. Source/include

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
yotta build
cp build/bbc-microbit-classic-gcc/Source/MicroBit.JS-combined.hex Build
