g++ -std=c++14 \
    tree/source/*.c log/source/*.c \
    tree/test/avl/*.cpp tree/test/btbox/*.cpp \
    -Itree/include -Ilog/include -Iutils/include\
    -o build/treetest \
    -lspdlog -lfmt -lgtest -lpthread

cd build
./treetest