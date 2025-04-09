g++ tree/source/*.cpp tree/test/*.cpp \
-o build/treetest \
-lspdlog -lfmt -lgtest -lpthread
cd build
./treetest