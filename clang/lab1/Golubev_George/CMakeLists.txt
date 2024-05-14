add_llvm_library(WarnDeprecatedFunction MODULE WarnDeprecatedFunction.cpp
                 PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS Support)
  clang_target_link_libraries(WarnDeprecatedFunction PRIVATE clangAST
                              clangBasic clangFrontend)
endif()
