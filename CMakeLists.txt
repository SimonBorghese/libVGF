cmake_minimum_required(VERSION 3.22)
project(libVGF C)

set(CMAKE_C_STANDARD 99)

include_directories(libVGF "lib")

add_executable(libVGF tests/test.c lib/structs.h lib/Read.c lib/Read.h lib/Write.c lib/Write.h lib/Common.c lib/Common.h)
