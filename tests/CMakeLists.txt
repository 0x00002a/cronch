
set(SRC 
    main.cpp
    serialize.cpp
        json_nloh.cpp

    )

add_executable(tests ${SRC})


find_package(doctest REQUIRED)

target_link_libraries(tests PUBLIC doctest::doctest cronch)

cronch_setup_target(tests)



