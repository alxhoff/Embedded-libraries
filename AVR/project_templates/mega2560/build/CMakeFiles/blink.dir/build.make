# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/alxhoff/git/GitHub/Teaching-AVR/Blink

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alxhoff/git/GitHub/Teaching-AVR/Blink/build

# Include any dependencies generated for this target.
include CMakeFiles/blink.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/blink.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blink.dir/flags.make

CMakeFiles/blink.dir/src/main.c.obj: CMakeFiles/blink.dir/flags.make
CMakeFiles/blink.dir/src/main.c.obj: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/GitHub/Teaching-AVR/Blink/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/blink.dir/src/main.c.obj"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blink.dir/src/main.c.obj   -c /home/alxhoff/git/GitHub/Teaching-AVR/Blink/src/main.c

CMakeFiles/blink.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blink.dir/src/main.c.i"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/GitHub/Teaching-AVR/Blink/src/main.c > CMakeFiles/blink.dir/src/main.c.i

CMakeFiles/blink.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blink.dir/src/main.c.s"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/GitHub/Teaching-AVR/Blink/src/main.c -o CMakeFiles/blink.dir/src/main.c.s

CMakeFiles/blink.dir/src/main.c.obj.requires:

.PHONY : CMakeFiles/blink.dir/src/main.c.obj.requires

CMakeFiles/blink.dir/src/main.c.obj.provides: CMakeFiles/blink.dir/src/main.c.obj.requires
	$(MAKE) -f CMakeFiles/blink.dir/build.make CMakeFiles/blink.dir/src/main.c.obj.provides.build
.PHONY : CMakeFiles/blink.dir/src/main.c.obj.provides

CMakeFiles/blink.dir/src/main.c.obj.provides.build: CMakeFiles/blink.dir/src/main.c.obj


# Object files for target blink
blink_OBJECTS = \
"CMakeFiles/blink.dir/src/main.c.obj"

# External object files for target blink
blink_EXTERNAL_OBJECTS =

blink.elf: CMakeFiles/blink.dir/src/main.c.obj
blink.elf: CMakeFiles/blink.dir/build.make
blink.elf: CMakeFiles/blink.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alxhoff/git/GitHub/Teaching-AVR/Blink/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable blink.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blink.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blink.dir/build: blink.elf

.PHONY : CMakeFiles/blink.dir/build

CMakeFiles/blink.dir/requires: CMakeFiles/blink.dir/src/main.c.obj.requires

.PHONY : CMakeFiles/blink.dir/requires

CMakeFiles/blink.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blink.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blink.dir/clean

CMakeFiles/blink.dir/depend:
	cd /home/alxhoff/git/GitHub/Teaching-AVR/Blink/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alxhoff/git/GitHub/Teaching-AVR/Blink /home/alxhoff/git/GitHub/Teaching-AVR/Blink /home/alxhoff/git/GitHub/Teaching-AVR/Blink/build /home/alxhoff/git/GitHub/Teaching-AVR/Blink/build /home/alxhoff/git/GitHub/Teaching-AVR/Blink/build/CMakeFiles/blink.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blink.dir/depend
