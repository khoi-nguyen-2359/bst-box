cmake_minimum_required(VERSION 3.14)
project(TreeView)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_library(TreeView ${SOURCES})

target_link_libraries(TreeView
   PRIVATE
        Logger
)