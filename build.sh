g++ bstbox/source/*.cpp tree/source/*.cpp \
-o build/bstbox \
-I/opt/homebrew/opt/spdlog/include -L/opt/homebrew/opt/spdlog/lib -lspdlog \
-I/opt/homebrew/opt/fmt/include -L/opt/homebrew/opt/fmt/lib -lfmt
