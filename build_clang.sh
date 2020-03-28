#!/bin/bash

set -e

if [ ! -d build ]; then
	mkdir build
	cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=./clang_toolchain.cmake ..
else
	cd build
fi

make "$@"
