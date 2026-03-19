add_llvm_library(UnusedPlugin MODULE
  UnusedAttrPlugin.cpp
  PLUGIN_TOOL clang
)
if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(UnusedPlugin PRIVATE clangFrontend clangAST clangBasic)
endif()
