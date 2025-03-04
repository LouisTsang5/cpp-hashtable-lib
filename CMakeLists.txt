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
    define_test(lookup_test)
    define_test(insertion_test)
endif()