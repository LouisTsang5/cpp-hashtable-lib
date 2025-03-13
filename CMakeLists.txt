# CMake Requirement
cmake_minimum_required(VERSION 3.12)

# C++ Requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project
project(HashTableLib VERSION 1.0 LANGUAGES CXX)

# Lib Target
add_library(hashtable INTERFACE)

# Include Dirs
target_include_directories(hashtable INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Run Teats
option(RUN_TESTS "Build and run tests" OFF)
if(RUN_TESTS)
    # Include CTest
    include(CTest)
    set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

    # Define add test routine
    function(define_test TEST_NAME)
        add_executable(${TEST_NAME} ${TEST_DIR}/${TEST_NAME}.cpp)
        target_include_directories(${TEST_NAME} PRIVATE ${TEST_DIR}/include)
        target_link_libraries(${TEST_NAME} PRIVATE hashtable)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endfunction()

    # Add unit tests
    define_test(kv_copy_test)
    define_test(kv_move_test)
    define_test(insertion_test)
    define_test(update_test)
    define_test(lookup_test)
endif()

# Run Benchmark
option(RUN_BENCHMARK "Build and run benchmark" OFF)
if(RUN_BENCHMARK)
    set(BM_DIR ${CMAKE_SOURCE_DIR}/benchmark)
    set(EX_LIB_DIR ${CMAKE_SOURCE_DIR}/extern)

    # Define add benchmark routine
    set(BENCHMARKS)
    function(define_bm BM_NAME)
        add_executable(${BM_NAME} ${BM_DIR}/${BM_NAME}.cpp)
        target_include_directories(${BM_NAME} PRIVATE ${EX_LIB_DIR})
        target_link_libraries(${BM_NAME} PRIVATE hashtable ${EX_LIB_DIR}/benchmark/libbenchmark.a)

        # Add to parent scope's list
        list(APPEND BENCHMARKS ${BM_NAME})
        set(BENCHMARKS ${BENCHMARKS} PARENT_SCOPE)
    endfunction()

    # Add benchmarks
    define_bm(benchmark_insertion)
    define_bm(benchmark_update)
    define_bm(benchmark_lookup)

    # Add target to run benchmarks
    add_custom_target(run_bm DEPENDS ${BENCHMARKS})
    foreach(BM ${BENCHMARKS})
        add_custom_command(TARGET run_bm
            POST_BUILD
            COMMAND ${CMAKE_BINARY_DIR}/${BM}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "\nRunning ${BM}..."
        )
    endforeach()
    set_target_properties(run_bm PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()