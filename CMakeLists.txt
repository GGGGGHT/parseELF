cmake_minimum_required(VERSION 3.22)
project(parseELF C)

set(CMAKE_C_STANDARD 11)

add_executable(parseELF  src/headers/linker.h src/paeseElf.c)
