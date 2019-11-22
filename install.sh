#!/bin/bash
PRO_DIR=`dirname $0`
if [ "$1" = "build" ]; then
    INSTALL_DIR="`dirname $0`/build"
else
    INSTALL_DIR=/opt/CuteTranslation
fi
mkdir -p ${INSTALL_DIR}
cp ${PRO_DIR}/python/*.py ${INSTALL_DIR}
cp ${PRO_DIR}/template/* ${INSTALL_DIR}
cp ${PRO_DIR}/build/CuteTranslation ${INSTALL_DIR}
mkdir -p ~/.config/CuteTranslation

