cmake_minimum_required(VERSION 3.22)
project(libVGF C)

set(CMAKE_C_STANDARD 99)

include_directories(libVGF "lib")

if (LIB)
    add_library(libVGF lib/structs.h lib/Read.c lib/Read.h lib/Write.c lib/Write.h lib/Common.c lib/Common.h lib/VGF.h)
else()
    add_executable(libVGF tests/test.c lib/structs.h lib/Read.c lib/Read.h lib/Write.c lib/Write.h lib/Common.c lib/Common.h lib/VGF.h)
endif()

target_compile_options(libVGF PRIVATE -Wall -Wextra -Werror -std=c99 -pedantic-errors)
