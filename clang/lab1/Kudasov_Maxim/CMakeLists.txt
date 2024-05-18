add_llvm_library(KickDeprecated MODULE KickDeprecated.cpp
                 PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
    set(LLVM_LINK_COMPONENTS Support)
    clang_target_link_libraries(KickDeprecated PRIVATE
        clangAST
        clangBasic
        clangFrontend
        llvmSupport
    )
endif()

set_target_properties(KickDeprecated
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)