cmake_minimum_required(VERSION 3.10)
project(mempool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the header-only library
add_library(mempool-lib INTERFACE)
target_include_directories(mempool-lib INTERFACE "src")
target_sources(mempool-lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/MemPool.cpp")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Define option for building tests, defaulting to ON if the project is the top-level project
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)    
    enable_testing()
    add_subdirectory(test)
else()
    option(BUILD_TESTS "Build tests" OFF)
endif()


