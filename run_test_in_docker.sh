#!/bin/bash

set -e

sudo apt update
sudo apt install python3 python3-pip python3-venv liblapacke-dev -y
python3 -m pip install --user pipx
python3 -m pipx ensurepath
export PATH="${PATH}:/home/dealii/.local/bin"
pipx install conan
conan profile new default
conan profile update settings.compiler=gcc default
conan profile update settings.compiler.version=9 default
conan profile update settings.arch=x86_64 default
conan profile update settings.os=Linux default
conan profile update settings.compiler.libcxx=libstdc++11 default

sudo update-alternatives --install /usr/local/bin/python python /usr/bin/python3 99

DEPS=/home/dealii/deps
INCLUDE_DIR=/home/dealii/include
INSTALL_DIR=/home/dealii/install
TEST_DIR=/home/dealii/test
TRAVIS_DIR=/home/travis

mkdir -p /home/dealii
mkdir $DEPS
mkdir $INCLUDE_DIR
mkdir $INSTALL_DIR
export PATH=$PATH:$INCLUDE_DIR:$INSTALL_DIR

cd $DEPS
git clone https://github.com/lubkoll/FunG
cd FunG && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR && cmake --build . --target install

cd $DEPS
git clone https://github.com/spacy-dev/Spacy
cd Spacy
mkdir build && cd build
conan install /home/spacy --build=missing
cmake /home/spacy -DdealII=ON -DBuildTest=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake
cmake --build . --target install

cd $DEPS/Spacy/Examples/deal.II_Tutorial/Tutorial/Step4 
mkdir build && cd build 
cmake .. && cmake --build . && ./run

cd $DEPS/Spacy/Examples/deal.II_Tutorial/Tutorial_Spacy/Step4 
mkdir build && cd build 
cmake .. && cmake --build . && ./run

mkdir $TEST_DIR && cd $TEST_DIR
mkdir build && cd build
conan install $TRAVIS_DIR
cmake $TRAVIS_DIR -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake
cmake --build .

ctest
