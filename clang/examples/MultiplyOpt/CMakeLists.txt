add_llvm_library(MultiplyOpt MODULE
  MultiplyOpt.cpp
  PLUGIN_TOOL opt
)

set(LIT_COMMAND "${LLVM_BINARY_DIR}/bin/llvm-lit" CACHE FILEPATH "Path to llvm-lit")
if (EXISTS ${LIT_COMMAND})
  add_custom_target(check-clang-examples-MultiplyOpt
    COMMAND ${LIT_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS MultiplyOpt
    COMMENT "Running lit tests for MultiplyOpt"
    USES_TERMINAL
  )
else()
  message(STATUS "llvm-lit not found, test target check-clang-examples-MultiplyOpt not created")
endif()
