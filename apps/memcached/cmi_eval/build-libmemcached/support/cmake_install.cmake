# Install script for directory: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/support

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aclocal" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/support/ax_libmemcached.m4")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libmemcached-awesome" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/example.cnf")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libmemcached.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libmemcached-awesome" TYPE FILE FILES
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libmemcached-config.cmake"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libhashkit-version.cmake"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libmemcached-version.cmake"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libmemcachedprotocol-version.cmake"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/libmemcachedutil-version.cmake"
    )
endif()

