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
CMAKE_SOURCE_DIR = /home/huw/projects/lightspark-wasm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/huw/projects/lightspark-wasm/obj-wasm

# Include any dependencies generated for this target.
include src/CMakeFiles/lightspark.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/lightspark.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/lightspark.dir/flags.make

src/CMakeFiles/lightspark.dir/main.cpp.o: src/CMakeFiles/lightspark.dir/flags.make
src/CMakeFiles/lightspark.dir/main.cpp.o: src/CMakeFiles/lightspark.dir/includes_CXX.rsp
src/CMakeFiles/lightspark.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huw/projects/lightspark-wasm/obj-wasm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/lightspark.dir/main.cpp.o"
	cd /home/huw/projects/lightspark-wasm/obj-wasm/src && /home/huw/emsdk/upstream/emscripten/em++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lightspark.dir/main.cpp.o -c /home/huw/projects/lightspark-wasm/src/main.cpp

src/CMakeFiles/lightspark.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lightspark.dir/main.cpp.i"
	cd /home/huw/projects/lightspark-wasm/obj-wasm/src && /home/huw/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huw/projects/lightspark-wasm/src/main.cpp > CMakeFiles/lightspark.dir/main.cpp.i

src/CMakeFiles/lightspark.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lightspark.dir/main.cpp.s"
	cd /home/huw/projects/lightspark-wasm/obj-wasm/src && /home/huw/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huw/projects/lightspark-wasm/src/main.cpp -o CMakeFiles/lightspark.dir/main.cpp.s

# Object files for target lightspark
lightspark_OBJECTS = \
"CMakeFiles/lightspark.dir/main.cpp.o"

# External object files for target lightspark
lightspark_EXTERNAL_OBJECTS =

x86/Debug/bin/lightspark.html: src/CMakeFiles/lightspark.dir/main.cpp.o
x86/Debug/bin/lightspark.html: src/CMakeFiles/lightspark.dir/build.make
x86/Debug/bin/lightspark.html: src/libspark.a
x86/Debug/bin/lightspark.html: src/CMakeFiles/lightspark.dir/linklibs.rsp
x86/Debug/bin/lightspark.html: src/CMakeFiles/lightspark.dir/objects1.rsp
x86/Debug/bin/lightspark.html: src/CMakeFiles/lightspark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/huw/projects/lightspark-wasm/obj-wasm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../x86/Debug/bin/lightspark.html"
	cd /home/huw/projects/lightspark-wasm/obj-wasm/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lightspark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/lightspark.dir/build: x86/Debug/bin/lightspark.html

.PHONY : src/CMakeFiles/lightspark.dir/build

src/CMakeFiles/lightspark.dir/clean:
	cd /home/huw/projects/lightspark-wasm/obj-wasm/src && $(CMAKE_COMMAND) -P CMakeFiles/lightspark.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/lightspark.dir/clean

src/CMakeFiles/lightspark.dir/depend:
	cd /home/huw/projects/lightspark-wasm/obj-wasm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/huw/projects/lightspark-wasm /home/huw/projects/lightspark-wasm/src /home/huw/projects/lightspark-wasm/obj-wasm /home/huw/projects/lightspark-wasm/obj-wasm/src /home/huw/projects/lightspark-wasm/obj-wasm/src/CMakeFiles/lightspark.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/lightspark.dir/depend
