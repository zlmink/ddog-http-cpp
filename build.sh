#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug --target all -- -j 10