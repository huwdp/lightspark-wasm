#!/bin/bash

# Stop on first error
set -e

if [[ $1 == "-h" ]] || [[ $1 == "--help" ]]; then
	echo "-h, --help	Print this message"
	echo "-d, --debug	Build Lightspark with debug symbols"
	echo "By default, this script does the release build of Lightspark."
	exit 0
fi

if ! type cmake > /dev/null 2>&1; then
	echo "cmake not found" >&2
	exit 1
fi
if ! type make > /dev/null 2>&1; then
	echo "make not found" >&2
	exit 1
fi

case "$1" in
	-d | --debug)
		mkdir -p obj-debug
		cd obj-debug
		cmake -DCMAKE_BUILD_TYPE=Debug ..
		;;
    --wasm)
        echo "Starting to compile for WASM"
        rm -rf obj-wasm
        mkdir -p obj-wasm
		cd obj-wasm
        
        # Helps that helped me
        # https://github.com/emscripten-core/emscripten/issues/9614#issuecomment-540867480
        export CXX=`echo clang | sed -e 's/clang/clang++/;s/gcc/g++/'`
        cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/home/huw/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -s USE_ZLIB=1 ..
		;;
    --clang)
        mkdir -p obj-clang
		cd obj-clang
        export CXX=`echo clang | sed -e 's/clang/clang++/;s/gcc/g++/'`
		cmake -DCMAKE_BUILD_TYPE=Debug ..
		;;
	*)
		mkdir -p obj-release
		cd obj-release
		cmake -DCMAKE_BUILD_TYPE=Release ..
		;;
esac

# Use all available threads
make -j $(grep -c processor /proc/cpuinfo)

#sudo make install
