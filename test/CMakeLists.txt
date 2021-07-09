
set(SRC main.cpp)

add_executable(tests ${SRC})

target_link_libraries(tests PUBLIC cronch)
