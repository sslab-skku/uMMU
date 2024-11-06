# Install script for directory: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached

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
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/include/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/contrib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/test/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libmemcached-awesome" TYPE FILE FILES
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/AUTHORS"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/BUGS.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/ChangeLog-0.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/ChangeLog-1.0.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/ChangeLog-1.1.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/CONTRIBUTING.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/LICENSE"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/NEWS"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/README.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/TODO"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libmemcached-awesome" TYPE FILE FILES
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/ChangeLog"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/ChangeLog.md"
    "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/COPYING"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
