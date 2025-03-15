cmake_minimum_required(VERSION 3.14)
project(Logger)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_library(Logger ${SOURCES})