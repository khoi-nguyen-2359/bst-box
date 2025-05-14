g++ -std=c++14 -g \
    tree/source/*.c log/source/*.c \
    tree/test/avl/*.cpp tree/test/btbox/*.cpp \
    -Itree/include -Ilog/include \
    -o build/treetest \
    -lspdlog -lfmt -lgtest -lpthread

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

cd build
./treetest