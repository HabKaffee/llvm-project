set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Lab1Plugin.exports)

add_llvm_library(Lab1Plugin MODULE
  Lab1Plugin.cpp

  PLUGIN_TOOL
  clang
)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS Support)
  clang_target_link_libraries(Lab1Plugin PRIVATE
    clangAST
    clangBasic
    clangFrontend
  )
endif()
