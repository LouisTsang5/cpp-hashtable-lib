#!/bin/sh
set -e # Exit if command failed
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build "build"
cmake -E chdir "build" ctest