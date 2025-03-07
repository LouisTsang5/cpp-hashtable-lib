# Hash Table

An opeen-address hash table implementation.

## Tests

Run the below commands to build and run tests

```bash
# Make a build dir named "build"
$ cmake -E make_directory "build"

# Config as tests
$ cmake -E chdir "build" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=DEBUG ..

# Build tests
$ cmake --build "build"

# Run tests
$ cmake -E chdir "build" ctest
```

Alternatively, run the [scripts/run_tests.sh](scripts/run_tests.sh) script to build and test
