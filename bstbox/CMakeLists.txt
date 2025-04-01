cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(BSTBox ${SOURCES})

target_link_libraries(BSTBox PRIVATE TreeView)
target_include_directories(BSTBox PRIVATE ${CMAKE_SOURCE_DIR}/treeview/source)
