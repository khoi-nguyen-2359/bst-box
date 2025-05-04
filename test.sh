g++ -std=c++14 \
    tree/source/*.cpp tree/source/*.c tree/test/*.cpp log/source/*.c \
    -Itree/include -Ilog/include \
    -o build/treetest \
    -lspdlog -lfmt -lgtest -lpthread

cd build
./treetest