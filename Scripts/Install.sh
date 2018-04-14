#!/usr/bin/env bash

mkdir Build

wget -O bootstrap.py https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py && echo '1 1' | python bootstrap.py
git clone --depth 1 https://github.com/mozilla/gecko-dev.git

cd gecko-dev/js/src
autoconf-2.13

mkdir ../../../Build/SpiderMonkey
cd ../../../Build/SpiderMonkey
../../gecko-dev/js/src/configure
make

cd ../../
pip install yotta
yotta target bbc-microbit-classic-gcc
