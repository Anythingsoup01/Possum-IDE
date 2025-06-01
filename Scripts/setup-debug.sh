#!/bin/bash

## THIS MUST BE RUN FROM THE ROOT DIRECTORY!

git submodule init && git submodule update

cd Ferret && git submodule init && git submodule update

cd ../ && mkdir build

cd build && cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug .
cmake --build . -j 8
