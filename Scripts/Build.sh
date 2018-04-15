#!/usr/bin/env bash

mkdir Build
mkdir Build/Duktape

wget http://duktape.org/duktape-2.2.0.tar.xz
tar xvfJ duktape-2.2.0.tar.xz
cd duktape-2.2.0
rm -rf src-custom
python tools/configure.py --output-directory ../Build/Duktape
cp -a Build/Duktape/. ./source
cd ../

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
yotta build
cp -a build/bbc-microbit-classic-gcc/source/. Build
