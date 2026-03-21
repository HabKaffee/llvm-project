set(LLVM_OPTIONAL_SOURCES
  SampleInput.cpp
  SampleInput2.cpp
)

add_llvm_library(ImplicitConversionsCounter MODULE ImplicitConversionsCounter.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(ImplicitConversionsCounter PRIVATE
    clangAST
    clangBasic
    clangFrontend
  )
endif()
