cmake_minimum_required(VERSION 3.2)
project(vmi C ASM_NASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "musl-gcc")
set(CMAKE_C_FLAGS "-static -O3 -mno-avx -mno-sse -mno-sse2")
# set(CMAKE_CXX_COMPILER "clang++")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../llvm-pass/internal)

add_subdirectory(src)

