#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/Diagnostic.h"
#include <iostream>

class FindDeprecatedFuncDeclVisitor
  : public clang::RecursiveASTVisitor<FindDeprecatedFuncDeclVisitor> {
private:
    clang::ASTContext& context;
    clang::CompilerInstance& compiler;
    clang::DiagnosticsEngine& engine;
public:
    explicit FindDeprecatedFuncDeclVisitor(
        clang::ASTContext& Context,
        clang::CompilerInstance& Compiler
        ) : context(Context),
            compiler(Compiler),
            engine(Compiler.getDiagnostics()) {}

    bool VisitFunctionDecl(clang::FunctionDecl *Declaration) {
        if (Declaration->getName().contains("deprecated")) {
            clang::FullSourceLoc loc = context.getFullLoc(Declaration->getLocation());
            unsigned int warningId = engine.getCustomDiagID(
                clang::DiagnosticsEngine::Warning,
                "This function-imposter contains \'deprecated\': \'%0\'"
            );
            engine.Report(loc, warningId) << Declaration->getNameAsString();
            std::cout << Declaration->getNameAsString() << std::endl;
        }
        return true;
    }
};

class KickDeprecatedWarnConsumer : public clang::ASTConsumer {
private:
    clang::CompilerInstance& compiler;
    FindDeprecatedFuncDeclVisitor visitor;
public:
    KickDeprecatedWarnConsumer(clang::CompilerInstance& Compiler)
        : compiler(Compiler), visitor(Compiler.getASTContext(), Compiler) {}

    virtual void HandleTranslationUnit(clang::ASTContext& Context) override {
        visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
};

class KickDeprecatedWarnAction : public clang::PluginASTAction {
protected:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &Compiler,
        llvm::StringRef InFile
    ) override {
        return std::make_unique<KickDeprecatedWarnConsumer>(Compiler);
    }

    bool ParseArgs(
        const clang::CompilerInstance &Compiler,
        const std::vector<std::string>& args
    ) override {
        return true;
    }
};

static clang::FrontendPluginRegistry::Add<KickDeprecatedWarnAction>
X("kick_deprecated_with_warn",
    "Pop up warning when clang detects function containing \'deprecated\' in name"
);