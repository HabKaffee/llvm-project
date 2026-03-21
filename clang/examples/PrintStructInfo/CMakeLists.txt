add_llvm_library(PrintStructInfoPlugin MODULE
   PrintStructInfo.cpp
)

set_target_properties(PrintStructInfoPlugin PROPERTIES
  PLUGIN_TOOL clang
  POSITION_INDEPENDENT_CODE ON
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(PrintStructInfoPlugin PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()