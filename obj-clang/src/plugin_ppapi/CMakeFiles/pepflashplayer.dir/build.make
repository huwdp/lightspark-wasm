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
CMAKE_BINARY_DIR = /home/huw/projects/lightspark-wasm/obj-clang

# Include any dependencies generated for this target.
include src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/depend.make

# Include the progress variables for this target.
include src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/progress.make

# Include the compile flags for this target's objects.
include src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/flags.make

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.o: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/flags.make
src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.o: ../src/plugin_ppapi/plugin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huw/projects/lightspark-wasm/obj-clang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.o"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pepflashplayer.dir/plugin.cpp.o -c /home/huw/projects/lightspark-wasm/src/plugin_ppapi/plugin.cpp

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pepflashplayer.dir/plugin.cpp.i"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huw/projects/lightspark-wasm/src/plugin_ppapi/plugin.cpp > CMakeFiles/pepflashplayer.dir/plugin.cpp.i

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pepflashplayer.dir/plugin.cpp.s"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huw/projects/lightspark-wasm/src/plugin_ppapi/plugin.cpp -o CMakeFiles/pepflashplayer.dir/plugin.cpp.s

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/flags.make
src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o: ../src/plugin_ppapi/ppextscriptobject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huw/projects/lightspark-wasm/obj-clang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o -c /home/huw/projects/lightspark-wasm/src/plugin_ppapi/ppextscriptobject.cpp

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.i"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huw/projects/lightspark-wasm/src/plugin_ppapi/ppextscriptobject.cpp > CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.i

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.s"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huw/projects/lightspark-wasm/src/plugin_ppapi/ppextscriptobject.cpp -o CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.s

# Object files for target pepflashplayer
pepflashplayer_OBJECTS = \
"CMakeFiles/pepflashplayer.dir/plugin.cpp.o" \
"CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o"

# External object files for target pepflashplayer
pepflashplayer_EXTERNAL_OBJECTS =

src/plugin_ppapi/libpepflashplayer.so: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/plugin.cpp.o
src/plugin_ppapi/libpepflashplayer.so: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/ppextscriptobject.cpp.o
src/plugin_ppapi/libpepflashplayer.so: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/build.make
src/plugin_ppapi/libpepflashplayer.so: src/liblightspark.so.0.8.4
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libz.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libfreetype.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libpng.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libz.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libfreetype.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libpng.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libpcre.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libGL.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libGLU.so
src/plugin_ppapi/libpepflashplayer.so: /usr/lib/x86_64-linux-gnu/libGLEW.so
src/plugin_ppapi/libpepflashplayer.so: src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/huw/projects/lightspark-wasm/obj-clang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared module libpepflashplayer.so"
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pepflashplayer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/build: src/plugin_ppapi/libpepflashplayer.so

.PHONY : src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/build

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/clean:
	cd /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi && $(CMAKE_COMMAND) -P CMakeFiles/pepflashplayer.dir/cmake_clean.cmake
.PHONY : src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/clean

src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/depend:
	cd /home/huw/projects/lightspark-wasm/obj-clang && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/huw/projects/lightspark-wasm /home/huw/projects/lightspark-wasm/src/plugin_ppapi /home/huw/projects/lightspark-wasm/obj-clang /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi /home/huw/projects/lightspark-wasm/obj-clang/src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/plugin_ppapi/CMakeFiles/pepflashplayer.dir/depend

