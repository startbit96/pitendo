# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/pi/Documents/repos/pitendo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Documents/repos/pitendo/build

# Include any dependencies generated for this target.
include CMakeFiles/pitendo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pitendo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pitendo.dir/flags.make

CMakeFiles/pitendo.dir/pitendo.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/pitendo.cpp.o: ../pitendo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pitendo.dir/pitendo.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/pitendo.cpp.o -c /home/pi/Documents/repos/pitendo/pitendo.cpp

CMakeFiles/pitendo.dir/pitendo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/pitendo.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/pitendo.cpp > CMakeFiles/pitendo.dir/pitendo.cpp.i

CMakeFiles/pitendo.dir/pitendo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/pitendo.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/pitendo.cpp -o CMakeFiles/pitendo.dir/pitendo.cpp.s

CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o: ../src/pitendo_game_engine/controller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o -c /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/controller.cpp

CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/controller.cpp > CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.i

CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/controller.cpp -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.s

CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o: ../src/pitendo_game_engine/display.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o -c /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/display.cpp

CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/display.cpp > CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.i

CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/display.cpp -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.s

CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o: ../src/pitendo_game_engine/pitendo_game_engine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o -c /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/pitendo_game_engine.cpp

CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/pitendo_game_engine.cpp > CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.i

CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/src/pitendo_game_engine/pitendo_game_engine.cpp -o CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.s

CMakeFiles/pitendo.dir/src/template/template.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/src/template/template.cpp.o: ../src/template/template.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/pitendo.dir/src/template/template.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/src/template/template.cpp.o -c /home/pi/Documents/repos/pitendo/src/template/template.cpp

CMakeFiles/pitendo.dir/src/template/template.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/src/template/template.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/src/template/template.cpp > CMakeFiles/pitendo.dir/src/template/template.cpp.i

CMakeFiles/pitendo.dir/src/template/template.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/src/template/template.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/src/template/template.cpp -o CMakeFiles/pitendo.dir/src/template/template.cpp.s

CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o: CMakeFiles/pitendo.dir/flags.make
CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o: ../src/flappy_bird/flappy_bird.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o -c /home/pi/Documents/repos/pitendo/src/flappy_bird/flappy_bird.cpp

CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/repos/pitendo/src/flappy_bird/flappy_bird.cpp > CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.i

CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/repos/pitendo/src/flappy_bird/flappy_bird.cpp -o CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.s

# Object files for target pitendo
pitendo_OBJECTS = \
"CMakeFiles/pitendo.dir/pitendo.cpp.o" \
"CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o" \
"CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o" \
"CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o" \
"CMakeFiles/pitendo.dir/src/template/template.cpp.o" \
"CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o"

# External object files for target pitendo
pitendo_EXTERNAL_OBJECTS =

pitendo: CMakeFiles/pitendo.dir/pitendo.cpp.o
pitendo: CMakeFiles/pitendo.dir/src/pitendo_game_engine/controller.cpp.o
pitendo: CMakeFiles/pitendo.dir/src/pitendo_game_engine/display.cpp.o
pitendo: CMakeFiles/pitendo.dir/src/pitendo_game_engine/pitendo_game_engine.cpp.o
pitendo: CMakeFiles/pitendo.dir/src/template/template.cpp.o
pitendo: CMakeFiles/pitendo.dir/src/flappy_bird/flappy_bird.cpp.o
pitendo: CMakeFiles/pitendo.dir/build.make
pitendo: CMakeFiles/pitendo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Documents/repos/pitendo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable pitendo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pitendo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pitendo.dir/build: pitendo

.PHONY : CMakeFiles/pitendo.dir/build

CMakeFiles/pitendo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pitendo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pitendo.dir/clean

CMakeFiles/pitendo.dir/depend:
	cd /home/pi/Documents/repos/pitendo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Documents/repos/pitendo /home/pi/Documents/repos/pitendo /home/pi/Documents/repos/pitendo/build /home/pi/Documents/repos/pitendo/build /home/pi/Documents/repos/pitendo/build/CMakeFiles/pitendo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pitendo.dir/depend

