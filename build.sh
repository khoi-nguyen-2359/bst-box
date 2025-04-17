OS_TYPE=$(uname | tr '[:upper:]' '[:lower:]')
if [ "$1" == "release" ]; then
    BUILD_FLAGS="-O3 -DNDEBUG"
    BUILD_TYPE="release"
else
    BUILD_FLAGS="-g -DDEBUG"
    BUILD_TYPE="debug"
fi
OUTPUT_FILE="build/bstbox_${BUILD_TYPE}_${OS_TYPE}"

mkdir build
g++ -std=c++11 bstbox/source/*.cpp tree/source/*.cpp \
-o $OUTPUT_FILE \
-lspdlog -lfmt \
$BUILD_FLAGS