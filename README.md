# Hash Table

An opeen-address hash table implementation.

## Tests

Run the below commands to build and run tests

```bash
# Make a build dir named "build/test"
$ cmake -E make_directory "build/test"

# Config as tests
$ cmake -E chdir "build/test" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=DEBUG ../../

# Build tests
$ cmake --build "build/test"

# Run tests
$ cmake -E chdir "build/test" ctest
```

Alternatively, run the [scripts/run_tests.sh](scripts/run_tests.sh) script to build and test

## Benchmark

### Dependency

The benchmark requires the [google benchmark](https://github.com/google/benchmark) library. 

It requires the below files from the library to be present at the ```${CMAKE_SOURCE_DIR}/extern/benchmark``` directory

- benchmark.h
- export.h
- libbenchmark.a

### Run the Benchmark

The below commands build and run the benchmark

```bash
# Make a build dir named "build/benchmark"
cmake -E make_directory "build/benchmark"

# Config as benchmark
cmake -E chdir "build/benchmark" cmake -DRUN_BENCHMARK=on -DCMAKE_BUILD_TYPE=RELEASE ../../

# Build and run benchmark
cmake --build "build/benchmark" --target run_bm
```

Alternatively, run the [scripts/run_benchmark.sh](scripts/run_benchmark.sh) script to build and test