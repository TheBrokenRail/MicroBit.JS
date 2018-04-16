#!/usr/bin/env bash

set -e

echo "Updating PIP"
sudo pip install --upgrade pip

echo "Generating C++ Source for JS File"
printf 'const char *jsSource = R"~~~~('"$(cat Source/main.js)"')~~~~";' > Source/JSSource.h

mkdir Build
mkdir Source/include

echo "Cloning mJS"
git clone --depth=1 https://github.com/cesanta/mjs.git
cp mjs/mjs.c Source/include
cp mjs/mjs.h Source/include

echo "Installing Yotta"
sudo pip install yotta
echo "Setting Yotta Target"
yotta target bbc-microbit-classic-gcc
echo "Building Project"
yotta build
echo "Copying Output to GH-Pages"
cp build/bbc-microbit-classic-gcc/Source/MicroBit-JS-combined.hex Build
