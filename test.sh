g++ -std=c++14 \
    tree/source/*.cpp tree/test/*.cpp log/source/*.cpp \
    -Itree/include -Ilog/include \
    -o build/treetest \
    -lspdlog -lfmt -lgtest -lpthread

cd build
./treetest