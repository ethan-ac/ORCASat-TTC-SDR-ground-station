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
CMAKE_SOURCE_DIR = /home/ethan/sdr-ground-station/gr-pduencode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ethan/sdr-ground-station/gr-pduencode/build

# Include any dependencies generated for this target.
include swig/CMakeFiles/pduencode_swig.dir/depend.make

# Include the progress variables for this target.
include swig/CMakeFiles/pduencode_swig.dir/progress.make

# Include the compile flags for this target's objects.
include swig/CMakeFiles/pduencode_swig.dir/flags.make

swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/pduencode_swig.dir/flags.make
swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ethan/sdr-ground-station/gr-pduencode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o"
	cd /home/ethan/sdr-ground-station/gr-pduencode/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o -c /home/ethan/sdr-ground-station/gr-pduencode/build/swig/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx

swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.i"
	cd /home/ethan/sdr-ground-station/gr-pduencode/build/swig && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ethan/sdr-ground-station/gr-pduencode/build/swig/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx > CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.i

swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.s"
	cd /home/ethan/sdr-ground-station/gr-pduencode/build/swig && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ethan/sdr-ground-station/gr-pduencode/build/swig/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx -o CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.s

# Object files for target pduencode_swig
pduencode_swig_OBJECTS = \
"CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o"

# External object files for target pduencode_swig
pduencode_swig_EXTERNAL_OBJECTS =

swig/_pduencode_swig.so: swig/CMakeFiles/pduencode_swig.dir/CMakeFiles/pduencode_swig.dir/pduencode_swigPYTHON_wrap.cxx.o
swig/_pduencode_swig.so: swig/CMakeFiles/pduencode_swig.dir/build.make
swig/_pduencode_swig.so: lib/libgnuradio-pduencode.so.v1.0-compat-xxx-xunknown
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libpython3.8.so
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so.3.8.2.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libgnuradio-pmt.so.3.8.2.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libvolk.so.2.2
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/liborc-0.4.so
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.71.0
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/liblog4cpp.so
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libgmpxx.so
swig/_pduencode_swig.so: /usr/lib/x86_64-linux-gnu/libgmp.so
swig/_pduencode_swig.so: swig/CMakeFiles/pduencode_swig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ethan/sdr-ground-station/gr-pduencode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module _pduencode_swig.so"
	cd /home/ethan/sdr-ground-station/gr-pduencode/build/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pduencode_swig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/CMakeFiles/pduencode_swig.dir/build: swig/_pduencode_swig.so

.PHONY : swig/CMakeFiles/pduencode_swig.dir/build

swig/CMakeFiles/pduencode_swig.dir/clean:
	cd /home/ethan/sdr-ground-station/gr-pduencode/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/pduencode_swig.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/pduencode_swig.dir/clean

swig/CMakeFiles/pduencode_swig.dir/depend:
	cd /home/ethan/sdr-ground-station/gr-pduencode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ethan/sdr-ground-station/gr-pduencode /home/ethan/sdr-ground-station/gr-pduencode/swig /home/ethan/sdr-ground-station/gr-pduencode/build /home/ethan/sdr-ground-station/gr-pduencode/build/swig /home/ethan/sdr-ground-station/gr-pduencode/build/swig/CMakeFiles/pduencode_swig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/pduencode_swig.dir/depend
