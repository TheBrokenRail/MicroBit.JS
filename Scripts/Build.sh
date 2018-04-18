#!/usr/bin/env bash

set -e

tput setaf 3; tput bold; echo "Generating C++ Source for JS File"
printf '#include <string>\nstd::string jsSource = R"~~~~('"$(cat Source/main.js)"')~~~~";' > Source/JSSource.h

tput setaf 3; tput bold; echo "Cloning mJS"
git clone --depth=1 https://github.com/cesanta/mjs.git
cp mjs/mjs.c Source
cp mjs/mjs.h Source

tput setaf 3; tput bold; echo "Installing Yotta"
pip install --user yotta
tput setaf 3; tput bold; echo "Creating Build Directory"
mkdir Build
tput setaf 3; tput bold; echo "Setting Yotta Target for BBC MicroBit"
yotta target bbc-microbit-classic-gcc
tput setaf 3; tput bold; echo "Building Project for BBC MicroBit"
yotta build
tput setaf 3; tput bold; echo "Setting Yotta Target for Linux"
yotta target x86-linux-native
tput setaf 3; tput bold; echo "Building Project for BBC Linux"
yotta build
tput setaf 3; tput bold; echo "Copying Output"
cp -r build/* Build
