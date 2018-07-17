cmake_minimum_required(VERSION 3.8)
project(memshare)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES memshare.c)
add_executable(memshare ${SOURCE_FILES})