set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/PrefixPlugin.exports)

add_llvm_library(PrefixPlugin MODULE PrefixPlugin.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(PrefixPlugin PRIVATE
    clangAST
    clangBasic
    clangFrontend
    clangRewrite
  )
endif()