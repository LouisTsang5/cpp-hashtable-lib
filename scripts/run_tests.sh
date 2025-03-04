#!/bin/sh

cmake -E make_directory "build"
cmake -E chdir "build" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=Release ..
cmake --build "build" --config release
cmake -E chdir "build" ctest --build-config Release