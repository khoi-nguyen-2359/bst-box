mkdir build
g++ -std=c++14 bstbox/source/*.cpp tree/source/*.cpp \
-o build/bstbox \
-lspdlog -lfmt