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
include SOIL/CMakeFiles/SOIL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include SOIL/CMakeFiles/SOIL.dir/compiler_depend.make

# Include the progress variables for this target.
include SOIL/CMakeFiles/SOIL.dir/progress.make

# Include the compile flags for this target's objects.
include SOIL/CMakeFiles/SOIL.dir/flags.make

SOIL/CMakeFiles/SOIL.dir/codegen:
.PHONY : SOIL/CMakeFiles/SOIL.dir/codegen

SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o: SOIL/CMakeFiles/SOIL.dir/flags.make
SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o: third_party/SOIL/src/image_dxt.c
SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o: SOIL/CMakeFiles/SOIL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o -MF CMakeFiles/SOIL.dir/src/image_dxt.c.o.d -o CMakeFiles/SOIL.dir/src/image_dxt.c.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_dxt.c

SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SOIL.dir/src/image_dxt.c.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_dxt.c > CMakeFiles/SOIL.dir/src/image_dxt.c.i

SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SOIL.dir/src/image_dxt.c.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_dxt.c -o CMakeFiles/SOIL.dir/src/image_dxt.c.s

SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o: SOIL/CMakeFiles/SOIL.dir/flags.make
SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o: third_party/SOIL/src/image_helper.c
SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o: SOIL/CMakeFiles/SOIL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o -MF CMakeFiles/SOIL.dir/src/image_helper.c.o.d -o CMakeFiles/SOIL.dir/src/image_helper.c.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_helper.c

SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SOIL.dir/src/image_helper.c.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_helper.c > CMakeFiles/SOIL.dir/src/image_helper.c.i

SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SOIL.dir/src/image_helper.c.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/image_helper.c -o CMakeFiles/SOIL.dir/src/image_helper.c.s

SOIL/CMakeFiles/SOIL.dir/src/soil.c.o: SOIL/CMakeFiles/SOIL.dir/flags.make
SOIL/CMakeFiles/SOIL.dir/src/soil.c.o: third_party/SOIL/src/soil.c
SOIL/CMakeFiles/SOIL.dir/src/soil.c.o: SOIL/CMakeFiles/SOIL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object SOIL/CMakeFiles/SOIL.dir/src/soil.c.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT SOIL/CMakeFiles/SOIL.dir/src/soil.c.o -MF CMakeFiles/SOIL.dir/src/soil.c.o.d -o CMakeFiles/SOIL.dir/src/soil.c.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/soil.c

SOIL/CMakeFiles/SOIL.dir/src/soil.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SOIL.dir/src/soil.c.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/soil.c > CMakeFiles/SOIL.dir/src/soil.c.i

SOIL/CMakeFiles/SOIL.dir/src/soil.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SOIL.dir/src/soil.c.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/soil.c -o CMakeFiles/SOIL.dir/src/soil.c.s

SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o: SOIL/CMakeFiles/SOIL.dir/flags.make
SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o: third_party/SOIL/src/stb_image_aug.c
SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o: SOIL/CMakeFiles/SOIL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o -MF CMakeFiles/SOIL.dir/src/stb_image_aug.c.o.d -o CMakeFiles/SOIL.dir/src/stb_image_aug.c.o -c /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/stb_image_aug.c

SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SOIL.dir/src/stb_image_aug.c.i"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/stb_image_aug.c > CMakeFiles/SOIL.dir/src/stb_image_aug.c.i

SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SOIL.dir/src/stb_image_aug.c.s"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL/src/stb_image_aug.c -o CMakeFiles/SOIL.dir/src/stb_image_aug.c.s

# Object files for target SOIL
SOIL_OBJECTS = \
"CMakeFiles/SOIL.dir/src/image_dxt.c.o" \
"CMakeFiles/SOIL.dir/src/image_helper.c.o" \
"CMakeFiles/SOIL.dir/src/soil.c.o" \
"CMakeFiles/SOIL.dir/src/stb_image_aug.c.o"

# External object files for target SOIL
SOIL_EXTERNAL_OBJECTS =

lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/src/image_dxt.c.o
lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/src/image_helper.c.o
lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/src/soil.c.o
lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/src/stb_image_aug.c.o
lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/build.make
lib/libSOIL.a: SOIL/CMakeFiles/SOIL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C static library ../lib/libSOIL.a"
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && $(CMAKE_COMMAND) -P CMakeFiles/SOIL.dir/cmake_clean_target.cmake
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SOIL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
SOIL/CMakeFiles/SOIL.dir/build: lib/libSOIL.a
.PHONY : SOIL/CMakeFiles/SOIL.dir/build

SOIL/CMakeFiles/SOIL.dir/clean:
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL && $(CMAKE_COMMAND) -P CMakeFiles/SOIL.dir/cmake_clean.cmake
.PHONY : SOIL/CMakeFiles/SOIL.dir/clean

SOIL/CMakeFiles/SOIL.dir/depend:
	cd /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/third_party/SOIL /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL /home/kituro/workflow/cs100/PA-cloned/CS100/PA/PA8/attachment/SOIL/CMakeFiles/SOIL.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : SOIL/CMakeFiles/SOIL.dir/depend

