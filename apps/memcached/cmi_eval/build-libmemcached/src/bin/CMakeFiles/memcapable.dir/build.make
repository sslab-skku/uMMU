# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached

# Include any dependencies generated for this target.
include src/bin/CMakeFiles/memcapable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/bin/CMakeFiles/memcapable.dir/compiler_depend.make

# Include the progress variables for this target.
include src/bin/CMakeFiles/memcapable.dir/progress.make

# Include the compile flags for this target's objects.
include src/bin/CMakeFiles/memcapable.dir/flags.make

src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o: src/bin/CMakeFiles/memcapable.dir/flags.make
src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memcapable.cc
src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o: src/bin/CMakeFiles/memcapable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o -MF CMakeFiles/memcapable.dir/memcapable.cc.o.d -o CMakeFiles/memcapable.dir/memcapable.cc.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memcapable.cc

src/bin/CMakeFiles/memcapable.dir/memcapable.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/memcapable.dir/memcapable.cc.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memcapable.cc > CMakeFiles/memcapable.dir/memcapable.cc.i

src/bin/CMakeFiles/memcapable.dir/memcapable.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/memcapable.dir/memcapable.cc.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memcapable.cc -o CMakeFiles/memcapable.dir/memcapable.cc.s

src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o: src/bin/CMakeFiles/memcapable.dir/flags.make
src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc
src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o: src/bin/CMakeFiles/memcapable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o -MF CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o.d -o CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc

src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc > CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.i

src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc -o CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.s

# Object files for target memcapable
memcapable_OBJECTS = \
"CMakeFiles/memcapable.dir/memcapable.cc.o" \
"CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o"

# External object files for target memcapable
memcapable_EXTERNAL_OBJECTS =

src/bin/memcapable: src/bin/CMakeFiles/memcapable.dir/memcapable.cc.o
src/bin/memcapable: src/bin/CMakeFiles/memcapable.dir/__/libmemcached/byteorder.cc.o
src/bin/memcapable: src/bin/CMakeFiles/memcapable.dir/build.make
src/bin/memcapable: src/bin/common/liblibclient_common.a
src/bin/memcapable: src/p9y/libp9y.a
src/bin/memcapable: src/libmemcached/libmemcached.so.11.0.0
src/bin/memcapable: src/libhashkit/libhashkit.so.2.0.0
src/bin/memcapable: src/bin/CMakeFiles/memcapable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable memcapable"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/memcapable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/bin/CMakeFiles/memcapable.dir/build: src/bin/memcapable
.PHONY : src/bin/CMakeFiles/memcapable.dir/build

src/bin/CMakeFiles/memcapable.dir/clean:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && $(CMAKE_COMMAND) -P CMakeFiles/memcapable.dir/cmake_clean.cmake
.PHONY : src/bin/CMakeFiles/memcapable.dir/clean

src/bin/CMakeFiles/memcapable.dir/depend:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin/CMakeFiles/memcapable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/bin/CMakeFiles/memcapable.dir/depend
