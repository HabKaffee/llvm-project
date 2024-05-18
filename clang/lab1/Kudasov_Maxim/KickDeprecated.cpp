#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

class FindDeprecatedFuncDeclVisitor
  : public clang::RecursiveASTVisitor<FindDeprecatedFuncDeclVisitor> {
public:
    FindDeprecatedFuncDeclVisitor(
        clang::ASTContext& Context,
        clang::CompilerInstance& Compiler
        ) : context(Context),
            compiler(Compiler),
            engine(Compiler.getDiagnostics()) {}

    bool VisitFunctionDecl(clang::FunctionDecl *Declaration) {
        if (Declaration->getName().contains("deprecated")) {
            clang::SourceLocation loc = Declaration->getLocation();
            unsigned int warningId = engine.getCustomDiagID(
                clang::DiagnosticsEngine::Warning,
                "This function-imposter contains \'deprecated\': \'%0\'"
            );
            engine.Report(loc, warningId) << Declaration->getNameAsString();
        }
        return true;
    }
private:
    clang::ASTContext& context;
    clang::CompilerInstance& compiler;
    clang::DiagnosticsEngine& engine;
};

class KickDeprecatedWarnConsumer : public clang::ASTConsumer {
public:
    KickDeprecatedWarnConsumer(clang::CompilerInstance& Compiler)
        : compiler(Compiler), visitor(Compiler.getASTContext(), Compiler) {}

    virtual void HandleTranslationUnit(clang::ASTContext& Context) override {
        visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
private:
    clang::CompilerInstance& compiler;
    FindDeprecatedFuncDeclVisitor visitor;
};

class KickDeprecatedWarnAction : public clang::PluginASTAction {
protected:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &Compiler,
        llvm::StringRef
    ) override {
        return std::make_unique<KickDeprecatedWarnConsumer>(Compiler);
    }

    bool ParseArgs(
        const clang::CompilerInstance &Compiler,
        const std::vector<std::string> &args
    ) override {
        return true;
    }
};

static clang::FrontendPluginRegistry::Add<KickDeprecatedWarnAction>
X("kick_deprecated_with_warn",
    "Pop up warning when clang detects function containing \'deprecated\' in name"
);