add_llvm_library(PrintClassNames MODULE PrintClassNames.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(PrintClassNames PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
