#!/bin/bash
set -e
cd build
qmake ..
make -j4
cd ..
sudo cp build/CuteTranslation deb/CuteTranslation/opt/CuteTranslation
sudo cp template/* deb/CuteTranslation/opt/CuteTranslation
sudo cp nodejs_binary/node_amd64 deb/CuteTranslation/opt/CuteTranslation/node
cd deb
sudo chown root.root -R ./CuteTranslation
dpkg -b ./CuteTranslation ./CuteTranslation_v0.1.0_amd64.deb
sudo chown jzc.jzc -R ./CuteTranslation
