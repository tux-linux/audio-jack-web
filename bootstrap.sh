#!/bin/bash

# Warning: this script will only work with Debian-based systems.
sudo apt install -y build-essential git libsndfile-dev bash qt5-qmake qtbase5-dev qtwebengine5-dev haskell-stack cmake libpulse-dev pavucontrol

cd cwwav/
make && cp -f cwwav ../scripts/client/cwwav
cd ..

cd pagerenc/
stack setup
stack build --local-bin-path ../scripts/server --copy-bins
cd ..

cd multimon-ng/
cmake .
make
cd ..

cd audio-jack-web-qt/
qmake
make
cd ..
