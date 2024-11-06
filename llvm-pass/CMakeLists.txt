cmake_minimum_required(VERSION 3.20.0)
project(CUMIRA)

find_package(LLVM 14 REQUIRED CONFIG)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

if (EXISTS "${SVF_DIR}")
else()
message(FATAL_ERROR "set SVF_DIR, ${SVF_DIR}")
endif()

set(SVF_LIB "${SVF_DIR}/Release-build/svf-llvm/libSvfLLVM.a")
include_directories("${SVF_DIR}/svf/include")
include_directories("${SVF_DIR}/svf-llvm/include")
include_directories("${SVF_DIR}/Release-build/include")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})



add_subdirectory(src)
