function(cronch_add_example NAME SRC)
  add_executable(${NAME} ${SRC})
  target_link_libraries(${NAME} PUBLIC cronch)
  message(STATUS "creating example: ${NAME}")

  cronch_add_werror(${NAME})
  cronch_add_diagnostics(${NAME})


endfunction()

cronch_add_example(to_json_basic serialization/basic_json.cpp)
cronch_add_example(from_json deserialization/json.cpp)

find_package(pugixml REQUIRED)

cronch_add_example(json_to_xml json_to_xml.cpp)
target_link_libraries(json_to_xml PUBLIC pugixml::pugixml)


cronch_add_example(props props.cpp)
cronch_add_example(custom_backend_plain custom_backend/plain.cpp)

