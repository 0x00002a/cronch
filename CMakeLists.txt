cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME cronch)
project(${PROJECT_NAME})


option(CRONCH_BUILD_TESTS "Whether to build the tests" OFF)
option(CRONCH_BUILD_EXAMPLES "Whether to build the examples" OFF)
option(CRONCH_BUILD_BENCHMARKS "Whether to build the benchmarks" OFF)
option(CRONCH_ADD_BUILD_TO_MODULE_PATH "Whether to add the build directory to the module path. You probably don't want this unless you're using conan" OFF)
option(CRONCH_USE_CONAN "Whether to pull in dependencies via conan" OFF)
option(CRONCH_FEATURE_BOOST_JSON "Whether to include boost.json support" ON)

if (CRONCH_ADD_BUILD_TO_MODULE_PATH)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
endif()

if (CRONCH_USE_CONAN)
    include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)
endif()

add_library(${PROJECT_NAME} INTERFACE)

if (CRONCH_FEATURE_BOOST_JSON)
    find_package(Boost 1.75.0 COMPONENTS json REQUIRED)
else()
    find_package(Boost 1.75.0 REQUIRED)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)


target_include_directories(cronch INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(cronch INTERFACE Boost::headers)

if (CRONCH_FEATURE_BOOST_JSON)
    target_link_libraries(cronch INTERFACE Boost::json)
    target_compile_definitions(cronch INTERFACE CRONCH_FEATURE_BOOST_JSON)
endif()

target_compile_features(cronch INTERFACE cxx_std_20)


target_compile_options(cronch INTERFACE
        $<$<BOOL:${MSVC}>:/Zc:preprocessor> # See: https://devblogs.microsoft.com/cppblog/announcing-full-support-for-a-c-c-conformant-preprocessor-in-msvc/
        )

if (CRONCH_BUILD_TESTS)
    add_subdirectory(tests)
endif()
if (CRONCH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (CRONCH_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()


