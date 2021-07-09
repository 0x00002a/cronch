function(cronch_add_example NAME SRC)
  add_executable(${NAME} ${SRC})
  target_link_libraries(${NAME} PUBLIC cronch)
  message(STATUS "creating example: ${NAME}")

endfunction()

cronch_add_example(json json.cpp)

find_package(pugixml REQUIRED)

cronch_add_example(json_to_xml json_to_xml.cpp)
target_link_libraries(json_to_xml PUBLIC pugixml::pugixml)
