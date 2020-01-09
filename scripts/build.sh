#!/bin/bash

cd ..
cmake -B cmake-build .
make -C cmake-build
cp scripts/81837.sh cmake-build