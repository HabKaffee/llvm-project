#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>

using namespace clang;

namespace {

class DeprecatedFunctionVisitor
    : public RecursiveASTVisitor<DeprecatedFunctionVisitor> {
public:
  explicit DeprecatedFunctionVisitor(ASTContext *Context) : Context(Context) {}

  bool VisitFunctionDecl(FunctionDecl *FD) {
    std::string actualFunctionName = FD->getNameAsString();

    if (actualFunctionName.find("deprecated") != std::string::npos ||
        actualFunctionName.find("Deprecated") != std::string::npos) {
      SourceLocation loc = FD->getLocation();
      llvm::errs() << "Warning: Function '" << actualFunctionName
                   << "' is deprecated\n";
      loc.print(llvm::errs(), Context->getSourceManager());
      llvm::errs() << "\n";
    }

    return true;
  }

private:
  ASTContext *Context;
};

class DeprecatedFunctionConsumer : public ASTConsumer {
public:
  explicit DeprecatedFunctionConsumer(ASTContext *Context) : Visitor(Context) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  DeprecatedFunctionVisitor Visitor;
};

class DeprecatedFunctionPlugin : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<DeprecatedFunctionConsumer>(&CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }
};

}

static FrontendPluginRegistry::Add<DeprecatedFunctionPlugin>
    X("deprecated-function-plugin",
      "Add warning on functions with 'deprecated' in their names");
