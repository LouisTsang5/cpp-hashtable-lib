#!/bin/sh
set -e # Exit if command failed
cmake -E make_directory "build/benchmark"
cmake -E chdir "build/benchmark" cmake -DRUN_BENCHMARK=on -DCMAKE_BUILD_TYPE=RELEASE ../../
cmake --build "build/benchmark" --target run_bm