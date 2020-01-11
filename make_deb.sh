#!/bin/bash
set -e
cd build
qmake ..
make
cd ..
cp build/CuteTranslation deb/CuteTranslation/opt/CuteTranslation
cp python/*.py deb/CuteTranslation/opt/CuteTranslation
cp template/* deb/CuteTranslation/opt/CuteTranslation
cd deb
chown root.root -R ./CuteTranslation
dpkg -b ./CuteTranslation ./CuteTranslation_v0.1.0_amd64.deb
