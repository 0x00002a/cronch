cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME cronch)
project(${PROJECT_NAME})


option(CRONCH_BUILD_TESTS "Whether to build the tests" OFF)
option(CRONCH_BUILD_EXAMPLES "Whether to build the examples" OFF)
option(CRONCH_BUILD_BENCHMARKS "Whether to build the benchmarks" OFF)


add_library(${PROJECT_NAME} INTERFACE)

find_package(Boost 1.75.0 REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)


target_include_directories(cronch INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(cronch INTERFACE Boost::headers)
target_compile_features(cronch INTERFACE cxx_std_20)

if (CRONCH_BUILD_TESTS)
    add_subdirectory(tests)
endif()
if (CRONCH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (CRONCH_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()
