#!/usr/bin/env bash

mkdir Build

wget -O bootstrap.py https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py && printf "1\n1\n1\n2\n" | python bootstrap.py
git clone --depth 1 https://github.com/mozilla/gecko-dev.git

cd gecko-dev/js/src
autoconf

mkdir ../../../Build/SpiderMonkey
cd ../../../Build/SpiderMonkey
../../gecko-dev/js/src/configure
make

cd ../../
pip install yotta
yotta target bbc-microbit-classic-gcc
