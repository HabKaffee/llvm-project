function(add_shad_executable NAME)
  add_executable(${NAME} ${ARGN})
  set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "-Wall -Werror -Wextra -Wpedantic")
endfunction()

function(add_catch TARGET)
  add_shad_executable(${TARGET} ${ARGN})
  target_link_libraries(${TARGET} PRIVATE Catch2::Catch2WithMain)
endfunction()
