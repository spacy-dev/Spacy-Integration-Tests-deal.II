#!/bin/bash

DEPS=$HOME/deps
INCLUDE_DIR=$HOME/include
LIB_DIR=$HOME/lib

mkdir $INCLUDE_DIR
mkdir $LIB_DIR
export PATH=$PATH:$INCLUDE_DIR:$LIB_DIR

cd $DEPS
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
cmake --build .
cp -r ../googletest/include/gtest $HOME/include
cp googlemock/gtest/lib*.a $HOME/lib

cd $DEPS
git clone https://github.com/lubkoll/FunG
cp -r FunG/fung $HOME/include

cd $DEPS
git clone https://github.com/spacy-dev/Spacy
cd Spacy
mkdir build
cd build
cmake .. -DdealII=ON -DBuildTest=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/lib
cmake --build .
make install

cd $HOME/test
mkdir build
cd build
cmake .. -DdealII=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .

ctest
