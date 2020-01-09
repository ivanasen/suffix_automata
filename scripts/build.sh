#!/bin/bash

cd ..
cmake -B cmake-build -DCMAKE_BUILD_TYPE=Release .
make -C cmake-build
cp scripts/81837.sh cmake-build
