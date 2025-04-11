OS_TYPE=$(uname | tr '[:upper:]' '[:lower:]')
if [ "$1" == "release" ]; then
    BUILD_FLAGS="-O3 -DNDEBUG"
    OUTPUT_FILE="build/bstbox_release_${OS_TYPE}"
else
    BUILD_FLAGS="-g -DDEBUG"
    OUTPUT_FILE="build/bstbox_debug_${OS_TYPE}"
fi

mkdir build
g++ -std=c++11 bstbox/source/*.cpp tree/source/*.cpp \
-o $OUTPUT_FILE \
-lspdlog -lfmt \
$BUILD_FLAGS