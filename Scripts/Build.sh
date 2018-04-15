#!/usr/bin/env bash

set -e

mkdir Build
mkdir Build/Duktape

wget http://duktape.org/duktape-2.2.0.tar.xz
tar xvfJ duktape-2.2.0.tar.xz
cd duktape-2.2.0
rm -rf src-custom
python tools/configure.py --output-directory ../Build/Duktape
cp -r ../Build/Duktape/. ./Source
cd ../

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
yotta build
cp -r build/bbc-microbit-classic-gcc/Source/. Build
