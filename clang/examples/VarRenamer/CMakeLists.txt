set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/VarRenamer.exports)

add_llvm_library(VarRenamer MODULE VarRenamer.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
set(LLVM_LINK_COMPONENTS Support)

target_link_libraries(VarRenamer PRIVATE 
	clangAST
	clangBasic
	clangFrontend
	clangRewrite
	clangSerialization
	clangTooling
	)
endif()