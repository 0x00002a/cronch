cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME cronch)
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)

find_package(Boost REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)

target_include_directories(cronch INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(cronch INTERFACE Boost::headers)
target_compile_features(cronch INTERFACE cxx_std_20)

add_subdirectory(tests)
add_subdirectory(examples)
