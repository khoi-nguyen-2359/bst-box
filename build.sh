#!/usr/bin/env bash

OS_TYPE=$(uname | tr '[:upper:]' '[:lower:]')

BUILD_FLAGS="-g"
BUILD_TYPE="_debug"
for arg in "$@"; do
    case $arg in
        release)
            BUILD_FLAGS="-O3"
            BUILD_TYPE=""
            ;;
    esac
done

OUTPUT_FILE="build/bstbox${BUILD_TYPE}_${OS_TYPE}"

if [[ ! -d build ]]; then
    mkdir build
fi

gcc -Werror \
    $BUILD_FLAGS \
    bstbox/source/*.c tree/source/*.c tools/source/*.c \
    -Itree/include -Itools/include \
    -o $OUTPUT_FILE \
    -lm

if [[ $? -eq 0 ]]; then
    echo "Build succeeded. Output file: $OUTPUT_FILE"
else
    echo "Build failed."
    exit 1
fi
