cmake_minimum_required(VERSION 3.14)
project(Process)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILE Process.cpp Process.h ProcessManager.h ProcessManager.cpp)

add_executable(Process main.cpp Process.cpp Process.h ProcessManager.h ProcessManager.cpp)