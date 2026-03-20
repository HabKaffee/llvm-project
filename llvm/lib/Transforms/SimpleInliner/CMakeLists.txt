add_llvm_component_library(LLVMSimpleInliner
  SimpleInliner.cpp

  LINK_COMPONENTS
  Core
  Support
  TransformUtils
  )
