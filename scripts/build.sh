#!/bin/bash

cd ..
cmake -B build -DCMAKE_BUILD_TYPE=Release .
make -C build
cp scripts/81837.sh build
