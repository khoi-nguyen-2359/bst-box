cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)

# add_subdirectory(test)

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(Visualizer ${SOURCES})

target_link_libraries(Visualizer PRIVATE TreeView)