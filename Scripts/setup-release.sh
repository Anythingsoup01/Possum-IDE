#!/bin/bash

cd ../ && mkdir build

cd build && cmake ..
cmake -DCMAKE_BUILD_TYPE=Release .
cmake --build . -j 8
