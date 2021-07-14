function(cronch_add_example NAME SRC)
  add_executable(${NAME} ${SRC})
  target_link_libraries(${NAME} PUBLIC cronch)
  message(STATUS "creating example: ${NAME}")

  cronch_setup_target(${NAME})


endfunction()

cronch_add_example(to_json_basic serialization/basic_json.cpp)
cronch_add_example(from_json deserialization/json.cpp)
cronch_add_example(json_to_xml json_to_xml.cpp)
cronch_add_example(props props.cpp)
cronch_add_example(custom_backend_plain custom_backend/plain.cpp)

