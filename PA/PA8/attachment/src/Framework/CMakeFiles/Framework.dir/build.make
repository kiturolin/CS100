# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_SOURCE_DIR = /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment

# Include any dependencies generated for this target.
include src/Framework/CMakeFiles/Framework.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/Framework/CMakeFiles/Framework.dir/compiler_depend.make

# Include the progress variables for this target.
include src/Framework/CMakeFiles/Framework.dir/progress.make

# Include the compile flags for this target's objects.
include src/Framework/CMakeFiles/Framework.dir/flags.make

src/Framework/CMakeFiles/Framework.dir/codegen:
.PHONY : src/Framework/CMakeFiles/Framework.dir/codegen

src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o: src/Framework/CMakeFiles/Framework.dir/flags.make
src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o: src/Framework/ObjectBase.cpp
src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o: src/Framework/CMakeFiles/Framework.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o -MF CMakeFiles/Framework.dir/ObjectBase.cpp.o.d -o CMakeFiles/Framework.dir/ObjectBase.cpp.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/ObjectBase.cpp

src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Framework.dir/ObjectBase.cpp.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/ObjectBase.cpp > CMakeFiles/Framework.dir/ObjectBase.cpp.i

src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Framework.dir/ObjectBase.cpp.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/ObjectBase.cpp -o CMakeFiles/Framework.dir/ObjectBase.cpp.s

src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o: src/Framework/CMakeFiles/Framework.dir/flags.make
src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o: src/Framework/TextBase.cpp
src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o: src/Framework/CMakeFiles/Framework.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o -MF CMakeFiles/Framework.dir/TextBase.cpp.o.d -o CMakeFiles/Framework.dir/TextBase.cpp.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/TextBase.cpp

src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Framework.dir/TextBase.cpp.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/TextBase.cpp > CMakeFiles/Framework.dir/TextBase.cpp.i

src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Framework.dir/TextBase.cpp.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/TextBase.cpp -o CMakeFiles/Framework.dir/TextBase.cpp.s

src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o: src/Framework/CMakeFiles/Framework.dir/flags.make
src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o: src/Framework/GameManager.cpp
src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o: src/Framework/CMakeFiles/Framework.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o -MF CMakeFiles/Framework.dir/GameManager.cpp.o.d -o CMakeFiles/Framework.dir/GameManager.cpp.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/GameManager.cpp

src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Framework.dir/GameManager.cpp.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/GameManager.cpp > CMakeFiles/Framework.dir/GameManager.cpp.i

src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Framework.dir/GameManager.cpp.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/GameManager.cpp -o CMakeFiles/Framework.dir/GameManager.cpp.s

src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o: src/Framework/CMakeFiles/Framework.dir/flags.make
src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o: src/Framework/SpriteManager.cpp
src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o: src/Framework/CMakeFiles/Framework.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o -MF CMakeFiles/Framework.dir/SpriteManager.cpp.o.d -o CMakeFiles/Framework.dir/SpriteManager.cpp.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/SpriteManager.cpp

src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Framework.dir/SpriteManager.cpp.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/SpriteManager.cpp > CMakeFiles/Framework.dir/SpriteManager.cpp.i

src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Framework.dir/SpriteManager.cpp.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/SpriteManager.cpp -o CMakeFiles/Framework.dir/SpriteManager.cpp.s

# Object files for target Framework
Framework_OBJECTS = \
"CMakeFiles/Framework.dir/ObjectBase.cpp.o" \
"CMakeFiles/Framework.dir/TextBase.cpp.o" \
"CMakeFiles/Framework.dir/GameManager.cpp.o" \
"CMakeFiles/Framework.dir/SpriteManager.cpp.o"

# External object files for target Framework
Framework_EXTERNAL_OBJECTS =

lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/ObjectBase.cpp.o
lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/TextBase.cpp.o
lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/GameManager.cpp.o
lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/SpriteManager.cpp.o
lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/build.make
lib/libFramework.a: src/Framework/CMakeFiles/Framework.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library ../../lib/libFramework.a"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && $(CMAKE_COMMAND) -P CMakeFiles/Framework.dir/cmake_clean_target.cmake
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Framework.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Framework/CMakeFiles/Framework.dir/build: lib/libFramework.a
.PHONY : src/Framework/CMakeFiles/Framework.dir/build

src/Framework/CMakeFiles/Framework.dir/clean:
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework && $(CMAKE_COMMAND) -P CMakeFiles/Framework.dir/cmake_clean.cmake
.PHONY : src/Framework/CMakeFiles/Framework.dir/clean

src/Framework/CMakeFiles/Framework.dir/depend:
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/src/Framework/CMakeFiles/Framework.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/Framework/CMakeFiles/Framework.dir/depend

