#!/bin/bash

DEPS=$HOME/deps
INCLUDE_DIR=$HOME/include
LIB_DIR=$HOME/lib
TEST_DIR=$HOME/test
TRAVIS_DIR=$HOME/travis

mkdir $DEPS
mkdir $INCLUDE_DIR
mkdir $LIB_DIR
export PATH=$PATH:$INCLUDE_DIR:$LIB_DIR

apt install libboost-dev
ls /usr/local/include
BOOST_INCLUDEDIR=/usr/include

cd $DEPS
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
cmake --build .
cmake --build . --target install

cd $DEPS
git clone https://github.com/lubkoll/FunG
cp -r FunG/fung $HOME/include

cd $DEPS
git clone https://github.com/spacy-dev/Spacy
cd Spacy
mkdir build && cd build
cmake .. -DdealII=ON -DBuildTest=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/lib
cmake --build .
cmake --build . --target install

mkdir $TEST_DIR && cd $TEST_DIR
mkdir build && cd build
cmake $TRAVIS_DIR -DdealII=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .

ctest
