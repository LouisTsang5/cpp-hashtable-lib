#!/bin/sh
set -e # Exit if command failed
cmake -E make_directory "build/test"
cmake -E chdir "build/test" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=DEBUG ../../
cmake --build "build/test"
cmake -E chdir "build/test" ctest