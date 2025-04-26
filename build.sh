OS_TYPE=$(uname | tr '[:upper:]' '[:lower:]')
if [ "$1" == "release" ]; then
    BUILD_FLAGS="-O3"
    BUILD_TYPE=""
else
    BUILD_FLAGS="-g -DLOGGABLE"
    BUILD_TYPE="_debug"
fi
OUTPUT_FILE="build/bstbox${BUILD_TYPE}_${OS_TYPE}"

mkdir build
g++ -std=c++11 \
$BUILD_FLAGS \
bstbox/source/*.cpp tree/source/*.cpp log/source/*.cpp \
-o $OUTPUT_FILE