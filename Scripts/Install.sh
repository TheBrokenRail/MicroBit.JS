#!/usr/bin/env bash

wget -O http://duktape.org/duktape-2.2.0.tar.xz
tar xvfJ duktape-2.2.0.tar.xz
cd duktape-2.2.0
rm -rf src-custom
python tools/configure.py
cp -a src-custom/. ./
cd ../

mkdir Build

sudo pip install yotta
yotta target bbc-microbit-classic-gcc
