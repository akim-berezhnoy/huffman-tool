#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

mkdir -p cmake-build-$1
rm -rf cmake-build-$1/*
cmake -DCMAKE_BUILD_TYPE=$1 -DENABLE_SLOW_TEST=ON -S . -B cmake-build-$1
cmake --build cmake-build-$1
