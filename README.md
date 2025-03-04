# Hash Table

An opeen-address hash table implementation.

## Tests

Run the below script to build and run tests

```bash
# Make a build dir named "build"
$ cmake -E make_directory "build"

# Config as tests
$ cmake -E chdir "build" cmake -DRUN_TESTS=on -DCMAKE_BUILD_TYPE=Release ..

# Build tests
$ cmake --build "build" --config release

# Run tests
$ cmake -E chdir "build" ctest --build-config Release
```