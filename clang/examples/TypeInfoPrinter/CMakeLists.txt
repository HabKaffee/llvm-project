add_llvm_library(TypeInfoPrinter MODULE TypeInfoPrinter.cpp PLUGIN_TOOL clang)

if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(TypeInfoPrinter PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
