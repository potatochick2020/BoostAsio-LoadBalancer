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
include CMakeFiles/MainServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MainServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MainServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MainServer.dir/flags.make

CMakeFiles/MainServer.dir/MainServer.cpp.o: CMakeFiles/MainServer.dir/flags.make
CMakeFiles/MainServer.dir/MainServer.cpp.o: /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/MainServer.cpp
CMakeFiles/MainServer.dir/MainServer.cpp.o: CMakeFiles/MainServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MainServer.dir/MainServer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MainServer.dir/MainServer.cpp.o -MF CMakeFiles/MainServer.dir/MainServer.cpp.o.d -o CMakeFiles/MainServer.dir/MainServer.cpp.o -c /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/MainServer.cpp

CMakeFiles/MainServer.dir/MainServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MainServer.dir/MainServer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/MainServer.cpp > CMakeFiles/MainServer.dir/MainServer.cpp.i

CMakeFiles/MainServer.dir/MainServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MainServer.dir/MainServer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/MainServer.cpp -o CMakeFiles/MainServer.dir/MainServer.cpp.s

# Object files for target MainServer
MainServer_OBJECTS = \
"CMakeFiles/MainServer.dir/MainServer.cpp.o"

# External object files for target MainServer
MainServer_EXTERNAL_OBJECTS =

MainServer: CMakeFiles/MainServer.dir/MainServer.cpp.o
MainServer: CMakeFiles/MainServer.dir/build.make
MainServer: CMakeFiles/MainServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MainServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MainServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MainServer.dir/build: MainServer
.PHONY : CMakeFiles/MainServer.dir/build

CMakeFiles/MainServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MainServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MainServer.dir/clean

CMakeFiles/MainServer.dir/depend:
	cd /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build /Users/akatsuki/Documents/GitHub/BoostAsio-LoadBalancer/build/CMakeFiles/MainServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MainServer.dir/depend

