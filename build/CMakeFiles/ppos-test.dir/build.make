# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nicolas/Downloads/PingPongOS-nicky

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicolas/Downloads/PingPongOS-nicky/build

# Include any dependencies generated for this target.
include CMakeFiles/ppos-test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ppos-test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ppos-test.dir/flags.make

CMakeFiles/ppos-test.dir/ppos-core-aux.c.o: CMakeFiles/ppos-test.dir/flags.make
CMakeFiles/ppos-test.dir/ppos-core-aux.c.o: ../ppos-core-aux.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/Downloads/PingPongOS-nicky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ppos-test.dir/ppos-core-aux.c.o"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ppos-test.dir/ppos-core-aux.c.o   -c /home/nicolas/Downloads/PingPongOS-nicky/ppos-core-aux.c

CMakeFiles/ppos-test.dir/ppos-core-aux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ppos-test.dir/ppos-core-aux.c.i"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nicolas/Downloads/PingPongOS-nicky/ppos-core-aux.c > CMakeFiles/ppos-test.dir/ppos-core-aux.c.i

CMakeFiles/ppos-test.dir/ppos-core-aux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ppos-test.dir/ppos-core-aux.c.s"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nicolas/Downloads/PingPongOS-nicky/ppos-core-aux.c -o CMakeFiles/ppos-test.dir/ppos-core-aux.c.s

CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o: CMakeFiles/ppos-test.dir/flags.make
CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o: ../pingpong-scheduler-srtf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/Downloads/PingPongOS-nicky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o   -c /home/nicolas/Downloads/PingPongOS-nicky/pingpong-scheduler-srtf.c

CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.i"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nicolas/Downloads/PingPongOS-nicky/pingpong-scheduler-srtf.c > CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.i

CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.s"
	/usr/bin/c89-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nicolas/Downloads/PingPongOS-nicky/pingpong-scheduler-srtf.c -o CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.s

# Object files for target ppos-test
ppos__test_OBJECTS = \
"CMakeFiles/ppos-test.dir/ppos-core-aux.c.o" \
"CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o"

# External object files for target ppos-test
ppos__test_EXTERNAL_OBJECTS =

ppos-test: CMakeFiles/ppos-test.dir/ppos-core-aux.c.o
ppos-test: CMakeFiles/ppos-test.dir/pingpong-scheduler-srtf.c.o
ppos-test: CMakeFiles/ppos-test.dir/build.make
ppos-test: ../libppos_static.a
ppos-test: CMakeFiles/ppos-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nicolas/Downloads/PingPongOS-nicky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ppos-test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ppos-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ppos-test.dir/build: ppos-test

.PHONY : CMakeFiles/ppos-test.dir/build

CMakeFiles/ppos-test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ppos-test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ppos-test.dir/clean

CMakeFiles/ppos-test.dir/depend:
	cd /home/nicolas/Downloads/PingPongOS-nicky/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolas/Downloads/PingPongOS-nicky /home/nicolas/Downloads/PingPongOS-nicky /home/nicolas/Downloads/PingPongOS-nicky/build /home/nicolas/Downloads/PingPongOS-nicky/build /home/nicolas/Downloads/PingPongOS-nicky/build/CMakeFiles/ppos-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ppos-test.dir/depend

