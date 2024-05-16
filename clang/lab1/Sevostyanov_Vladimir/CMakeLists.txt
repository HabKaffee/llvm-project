

add_llvm_library(AddAttribute MODULE AddAttribute.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(AddAttribute PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
