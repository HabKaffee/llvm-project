add_llvm_library(PrintStructInfo MODULE PrintStructInfo.cpp)

set_target_properties(PrintStructInfo PROPERTIES FOLDER "Clang examples")
install(TARGETS PrintStructInfo
  COMPONENT PrintStructInfo
  LIBRARY DESTINATION lib${LLVM_LIBDIR_SUFFIX}
  ARCHIVE DESTINATION lib${LLVM_LIBDIR_SUFFIX}
  RUNTIME DESTINATION bin
)
