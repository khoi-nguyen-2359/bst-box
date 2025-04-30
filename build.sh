OS_TYPE=$(uname | tr '[:upper:]' '[:lower:]')

BUILD_FLAGS="-g"
BUILD_TYPE="_debug"
for arg in "$@"; do
    case $arg in
        release)
            BUILD_FLAGS="-O3"
            BUILD_TYPE=""
            ;;
        loggable)
            BUILD_FLAGS="$BUILD_FLAGS -DLOGGABLE"
            ;;
    esac
done

OUTPUT_FILE="build/bstbox${BUILD_TYPE}_${OS_TYPE}"

if [[ ! -d build ]]; then
    mkdir build
fi

g++ -std=c++11 \
$BUILD_FLAGS \
bstbox/source/*.cpp tree/source/*.cpp log/source/*.cpp \
-o $OUTPUT_FILE

if [[ $? -eq 0 ]]; then
    echo "Build succeeded. Output file: $OUTPUT_FILE"
else
    echo "Build failed."
    exit 1
fi
