
function(cronch_add_werror TARGET)
    if (MSVC) 
        target_compile_options( 
            ${TARGET}
            PRIVATE 
                /Wall 
                /WX
            )
    else()
        target_compile_options(
            ${TARGET}
            PRIVATE 
                -Werror 
                -Wall 
                -Wextra 
                -Wpedantic
            )

    endif() 

endfunction()

function(cronch_add_diagnostics TARGET)

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "(GNU|AppleClang|Clang)")
        target_compile_options( 
            ${TARGET}
            PRIVATE 
                -fdiagnostics-color=always
                -fdiagnostics-show-template-tree
            )
    endif()

endfunction()


function(cronch_link_deps TARGET) 

    find_package(nlohmann_json REQUIRED)
    find_package(pugixml REQUIRED)
    target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json pugixml::pugixml)

endfunction()


function(cronch_setup_target TARGET)

    cronch_add_diagnostics(${TARGET})
    cronch_add_werror(${TARGET})
    cronch_link_deps(${TARGET})

endfunction()

