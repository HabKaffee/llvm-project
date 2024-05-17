add_llvm_library(WarnDeprecated MODULE WarnDeprecated.cpp
                 PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
    set(LLVM_LINK_COMPONENTS Support)
        clang_target_link_libraries(WarnDeprecated PRIVATE clangAST
                                    clangBasic clangFrontend)
endif()

set_target_properties(WarnDeprecated
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)