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

    # Add unit tests
    add_executable(unit_tests tests/unit_tests.cpp)
    target_link_libraries(unit_tests PRIVATE hashtable)
    add_test(NAME unit_tests COMMAND unit_tests)
endif()