add_llvm_library(CastCounter MODULE
  CastCounter.cpp
  PLUGIN_TOOL clang
)

if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(CastCounter PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
