#!/usr/bin/env bash
set -e

cd "$(dirname "${0}")"
if [ ! -d "build" ]; then
  cmake -B build
fi
cmake --build build
echo =======================================
./build/oop