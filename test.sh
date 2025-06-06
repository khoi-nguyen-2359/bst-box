#!/usr/bin/env bash

if [[ ! -d build ]]; then
    mkdir build
fi

g++ -g \
    tree/source/*.c tools/source/*.c \
    tools/test/*.cpp tree/test/avl/*.cpp tree/test/btbox/*.cpp \
    -Itree/include -Itools/include \
    -o build/treetest \
    -lfmt -lgtest -lpthread

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

cd build
./treetest