#!/usr/bin/env bash

set -e

printf 'std::string dataBaseLeaf = R"~~~~(\n'"$(cat Source/main.js)"'\n)~~~~";' > Source/JSSource.h

mkdir Build
mkdir Source/include

git clone --depth=1 https://github.com/cesanta/mjs.git
cp mjs/mjs.c Source/include
cp mjs/mjs.h Source/include

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
yotta build
cp build/bbc-microbit-classic-gcc/Source/MicroBit-JS-combined.hex Build
