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
CMAKE_SOURCE_DIR = /home/ludlin-8/Documents/gitrepos/rasterizer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ludlin-8/Documents/gitrepos/rasterizer

# Include any dependencies generated for this target.
include engine/render/CMakeFiles/render.dir/depend.make

# Include the progress variables for this target.
include engine/render/CMakeFiles/render.dir/progress.make

# Include the compile flags for this target's objects.
include engine/render/CMakeFiles/render.dir/flags.make

engine/render/CMakeFiles/render.dir/window.cc.o: engine/render/CMakeFiles/render.dir/flags.make
engine/render/CMakeFiles/render.dir/window.cc.o: engine/render/window.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ludlin-8/Documents/gitrepos/rasterizer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object engine/render/CMakeFiles/render.dir/window.cc.o"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/render.dir/window.cc.o -c /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render/window.cc

engine/render/CMakeFiles/render.dir/window.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render.dir/window.cc.i"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render/window.cc > CMakeFiles/render.dir/window.cc.i

engine/render/CMakeFiles/render.dir/window.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render.dir/window.cc.s"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render/window.cc -o CMakeFiles/render.dir/window.cc.s

engine/render/CMakeFiles/render.dir/__/config.cc.o: engine/render/CMakeFiles/render.dir/flags.make
engine/render/CMakeFiles/render.dir/__/config.cc.o: engine/config.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ludlin-8/Documents/gitrepos/rasterizer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object engine/render/CMakeFiles/render.dir/__/config.cc.o"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/render.dir/__/config.cc.o -c /home/ludlin-8/Documents/gitrepos/rasterizer/engine/config.cc

engine/render/CMakeFiles/render.dir/__/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render.dir/__/config.cc.i"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ludlin-8/Documents/gitrepos/rasterizer/engine/config.cc > CMakeFiles/render.dir/__/config.cc.i

engine/render/CMakeFiles/render.dir/__/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render.dir/__/config.cc.s"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ludlin-8/Documents/gitrepos/rasterizer/engine/config.cc -o CMakeFiles/render.dir/__/config.cc.s

# Object files for target render
render_OBJECTS = \
"CMakeFiles/render.dir/window.cc.o" \
"CMakeFiles/render.dir/__/config.cc.o"

# External object files for target render
render_EXTERNAL_OBJECTS =

engine/render/librender.a: engine/render/CMakeFiles/render.dir/window.cc.o
engine/render/librender.a: engine/render/CMakeFiles/render.dir/__/config.cc.o
engine/render/librender.a: engine/render/CMakeFiles/render.dir/build.make
engine/render/librender.a: engine/render/CMakeFiles/render.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ludlin-8/Documents/gitrepos/rasterizer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library librender.a"
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && $(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean_target.cmake
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/render.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
engine/render/CMakeFiles/render.dir/build: engine/render/librender.a

.PHONY : engine/render/CMakeFiles/render.dir/build

engine/render/CMakeFiles/render.dir/clean:
	cd /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render && $(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean.cmake
.PHONY : engine/render/CMakeFiles/render.dir/clean

engine/render/CMakeFiles/render.dir/depend:
	cd /home/ludlin-8/Documents/gitrepos/rasterizer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ludlin-8/Documents/gitrepos/rasterizer /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render /home/ludlin-8/Documents/gitrepos/rasterizer /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render /home/ludlin-8/Documents/gitrepos/rasterizer/engine/render/CMakeFiles/render.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : engine/render/CMakeFiles/render.dir/depend

