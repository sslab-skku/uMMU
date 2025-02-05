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
include src/bin/CMakeFiles/memping.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/bin/CMakeFiles/memping.dir/compiler_depend.make

# Include the progress variables for this target.
include src/bin/CMakeFiles/memping.dir/progress.make

# Include the compile flags for this target's objects.
include src/bin/CMakeFiles/memping.dir/flags.make

src/bin/CMakeFiles/memping.dir/memping.cc.o: src/bin/CMakeFiles/memping.dir/flags.make
src/bin/CMakeFiles/memping.dir/memping.cc.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memping.cc
src/bin/CMakeFiles/memping.dir/memping.cc.o: src/bin/CMakeFiles/memping.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/bin/CMakeFiles/memping.dir/memping.cc.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/bin/CMakeFiles/memping.dir/memping.cc.o -MF CMakeFiles/memping.dir/memping.cc.o.d -o CMakeFiles/memping.dir/memping.cc.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memping.cc

src/bin/CMakeFiles/memping.dir/memping.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/memping.dir/memping.cc.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memping.cc > CMakeFiles/memping.dir/memping.cc.i

src/bin/CMakeFiles/memping.dir/memping.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/memping.dir/memping.cc.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin/memping.cc -o CMakeFiles/memping.dir/memping.cc.s

# Object files for target memping
memping_OBJECTS = \
"CMakeFiles/memping.dir/memping.cc.o"

# External object files for target memping
memping_EXTERNAL_OBJECTS =

src/bin/memping: src/bin/CMakeFiles/memping.dir/memping.cc.o
src/bin/memping: src/bin/CMakeFiles/memping.dir/build.make
src/bin/memping: src/bin/common/liblibclient_common.a
src/bin/memping: src/libmemcachedutil/libmemcachedutil.so.2.0.0
src/bin/memping: src/p9y/libp9y.a
src/bin/memping: src/libmemcached/libmemcached.so.11.0.0
src/bin/memping: src/libhashkit/libhashkit.so.2.0.0
src/bin/memping: src/bin/CMakeFiles/memping.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable memping"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/memping.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/bin/CMakeFiles/memping.dir/build: src/bin/memping
.PHONY : src/bin/CMakeFiles/memping.dir/build

src/bin/CMakeFiles/memping.dir/clean:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin && $(CMAKE_COMMAND) -P CMakeFiles/memping.dir/cmake_clean.cmake
.PHONY : src/bin/CMakeFiles/memping.dir/clean

src/bin/CMakeFiles/memping.dir/depend:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/bin /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/bin/CMakeFiles/memping.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/bin/CMakeFiles/memping.dir/depend

