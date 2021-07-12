
set(SRC 
    main.cpp
    serialize.cpp

    )

add_executable(tests ${SRC})


find_package(doctest REQUIRED)
find_package(pugixml REQUIRED)

target_link_libraries(tests PUBLIC doctest::doctest cronch pugixml::pugixml)

cronch_add_werror(tests)
cronch_add_diagnostics(tests)



