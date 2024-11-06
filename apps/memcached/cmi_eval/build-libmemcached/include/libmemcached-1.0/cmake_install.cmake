# Install script for directory: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/include/libmemcached-1.0/struct/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/include/libmemcached-1.0/types/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/include/libmemcached-1.0/configure.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/alloc.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/allocators.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/analyze.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/auto.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/basic_string.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/behavior.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/callback.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/callbacks.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/defaults.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/delete.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/deprecated_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/dump.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/encoding_key.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/error.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/exception.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/exist.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/fetch.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/flush_buffers.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/flush.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/get.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/hash.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/limits.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/memcached.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/memcached.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/options.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/parse.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/platform.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/quit.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/result.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/return.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/sasl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/server.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/server_list.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/stats.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/storage.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/strerror.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/touch.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/triggers.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/verbosity.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libmemcached-1.0" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/include/libmemcached-1.0/visibility.h")
endif()

