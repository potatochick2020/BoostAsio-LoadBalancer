# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build

# Include any dependencies generated for this target.
include CMakeFiles/Worker_Server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Worker_Server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Worker_Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Worker_Server.dir/flags.make

CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o: CMakeFiles/Worker_Server.dir/flags.make
CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o: /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/Worker_Server.cpp
CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o: CMakeFiles/Worker_Server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o -MF CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o.d -o CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o -c /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/Worker_Server.cpp

CMakeFiles/Worker_Server.dir/Worker_Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Worker_Server.dir/Worker_Server.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/Worker_Server.cpp > CMakeFiles/Worker_Server.dir/Worker_Server.cpp.i

CMakeFiles/Worker_Server.dir/Worker_Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Worker_Server.dir/Worker_Server.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/Worker_Server.cpp -o CMakeFiles/Worker_Server.dir/Worker_Server.cpp.s

# Object files for target Worker_Server
Worker_Server_OBJECTS = \
"CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o"

# External object files for target Worker_Server
Worker_Server_EXTERNAL_OBJECTS =

Worker_Server: CMakeFiles/Worker_Server.dir/Worker_Server.cpp.o
Worker_Server: CMakeFiles/Worker_Server.dir/build.make
Worker_Server: CMakeFiles/Worker_Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Worker_Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Worker_Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Worker_Server.dir/build: Worker_Server
.PHONY : CMakeFiles/Worker_Server.dir/build

CMakeFiles/Worker_Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Worker_Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Worker_Server.dir/clean

CMakeFiles/Worker_Server.dir/depend:
	cd /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles/Worker_Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Worker_Server.dir/depend
