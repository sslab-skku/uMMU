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
include src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.make

# Include the progress variables for this target.
include src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/progress.make

# Include the compile flags for this target's objects.
include src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/ascii_handler.c
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o -MF CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o.d -o CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/ascii_handler.c

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/ascii_handler.c > CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/ascii_handler.c -o CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.s

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/binary_handler.c
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o -MF CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o.d -o CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/binary_handler.c

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/binary_handler.c > CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/binary_handler.c -o CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.s

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/cache.c
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o -MF CMakeFiles/libmemcachedprotocol.dir/cache.c.o.d -o CMakeFiles/libmemcachedprotocol.dir/cache.c.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/cache.c

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libmemcachedprotocol.dir/cache.c.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/cache.c > CMakeFiles/libmemcachedprotocol.dir/cache.c.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libmemcachedprotocol.dir/cache.c.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/cache.c -o CMakeFiles/libmemcachedprotocol.dir/cache.c.s

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/handler.c
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o -MF CMakeFiles/libmemcachedprotocol.dir/handler.c.o.d -o CMakeFiles/libmemcachedprotocol.dir/handler.c.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/handler.c

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libmemcachedprotocol.dir/handler.c.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/handler.c > CMakeFiles/libmemcachedprotocol.dir/handler.c.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libmemcachedprotocol.dir/handler.c.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/handler.c -o CMakeFiles/libmemcachedprotocol.dir/handler.c.s

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/pedantic.c
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o -MF CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o.d -o CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/pedantic.c

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libmemcachedprotocol.dir/pedantic.c.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/pedantic.c > CMakeFiles/libmemcachedprotocol.dir/pedantic.c.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libmemcachedprotocol.dir/pedantic.c.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol/pedantic.c -o CMakeFiles/libmemcachedprotocol.dir/pedantic.c.s

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/flags.make
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o: /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o -MF CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o.d -o CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o -c /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.i"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc > CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.i

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.s"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcached/byteorder.cc -o CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.s

# Object files for target libmemcachedprotocol
libmemcachedprotocol_OBJECTS = \
"CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o" \
"CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o" \
"CMakeFiles/libmemcachedprotocol.dir/cache.c.o" \
"CMakeFiles/libmemcachedprotocol.dir/handler.c.o" \
"CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o" \
"CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o"

# External object files for target libmemcachedprotocol
libmemcachedprotocol_EXTERNAL_OBJECTS =

src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/ascii_handler.c.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/binary_handler.c.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/cache.c.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/handler.c.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/pedantic.c.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/__/libmemcached/byteorder.cc.o
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/build.make
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/p9y/libp9y.a
src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0: src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX shared library libmemcachedprotocol.so"
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libmemcachedprotocol.dir/link.txt --verbose=$(VERBOSE)
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && $(CMAKE_COMMAND) -E cmake_symlink_library libmemcachedprotocol.so.0.0.0 libmemcachedprotocol.so.0 libmemcachedprotocol.so

src/libmemcachedprotocol/libmemcachedprotocol.so.0: src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate src/libmemcachedprotocol/libmemcachedprotocol.so.0

src/libmemcachedprotocol/libmemcachedprotocol.so: src/libmemcachedprotocol/libmemcachedprotocol.so.0.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate src/libmemcachedprotocol/libmemcachedprotocol.so

# Rule to build all files generated by this target.
src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/build: src/libmemcachedprotocol/libmemcachedprotocol.so
.PHONY : src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/build

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/clean:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol && $(CMAKE_COMMAND) -P CMakeFiles/libmemcachedprotocol.dir/cmake_clean.cmake
.PHONY : src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/clean

src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/depend:
	cd /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/libmemcached/src/libmemcachedprotocol /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol /home/hjoll6/Workspace/CMI-project/apps/memcached/cmi_eval/build-libmemcached/src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/libmemcachedprotocol/CMakeFiles/libmemcachedprotocol.dir/depend

