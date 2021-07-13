
set(SRC 
    main.cpp
    json.cpp
    xml.cpp
    types.hpp
    )

set(BUILD_TYPE_CACHE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)

add_executable(bmarks ${SRC})

find_package(benchmark REQUIRED)
find_package(pugixml REQUIRED)

target_link_libraries(bmarks PUBLIC cronch benchmark::benchmark pugixml::pugixml)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "(GNU|AppleClang|Clang)")
    target_compile_options(bmarks PRIVATE -fno-omit-frame-pointer)
endif()

set(CMAKE_BUILD_TYPE ${BUILD_TYPE_CACHE})


cronch_add_werror(bmarks)
cronch_add_diagnostics(bmarks)

