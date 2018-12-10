#!/bin/bash

DEPS=$HOME/deps
INCLUDE_DIR=$HOME/include
INSTALL_DIR=$HOME/install
TEST_DIR=$HOME/test
TRAVIS_DIR=$HOME/travis

mkdir $DEPS
mkdir $INCLUDE_DIR
mkdir $INSTALL_DIR
export PATH=$PATH:$INCLUDE_DIR:$INSTALL_DIR

cd $DEPS
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
cmake --build .
cmake --build . --target install

cd $DEPS
git clone https://github.com/lubkoll/FunG
cd FunG && mkdir build && build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR && make install

cd $DEPS
git clone https://github.com/spacy-dev/Spacy
cd Spacy
mkdir build && cd build
cmake .. -DdealII=ON -DBuildTest=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
cmake --build .
cmake --build . --target install

cd $DEPS/Spacy/Examples/deal.II_Tutorial/Tutorial/Step4 
mkdir build && cd build 
cmake .. && cmake --build . && ./run

cd $DEPS/Spacy/Examples/deal.II_Tutorial/Tutorial_Spacy/Step4 
mkdir build && cd build 
cmake .. && cmake --build . && ./run

mkdir $TEST_DIR && cd $TEST_DIR
mkdir build && cd build
cmake $TRAVIS_DIR -DdealII=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .

ctest
