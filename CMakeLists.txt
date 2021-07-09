cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME cronch)
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)

find_library(Boost REQUIRED)

target_include_directories(cronch PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(cronch PUBLIC Boost::headers)

# add_subdirectory(test)
add_subdirectory(examples)
