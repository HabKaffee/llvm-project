#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"

class DeprecatedFunctionVisitor : public clang::RecursiveASTVisitor<DeprecatedFunctionVisitor> {
public:
  explicit DeprecatedFunctionVisitor(clang::CompilerInstance &CI)
      : CI(CI) {}

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->getName().contains("deprecated")) {
      clang::DiagnosticsEngine &Diags = CI.getDiagnostics();
      unsigned DiagID = Diags.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                               "Function is deprecated");
      Diags.Report(FD->getLocation(), DiagID);
    }
    return true;
  }

private:
  clang::CompilerInstance &CI;
};

class DeprecatedFunctionConsumer : public clang::ASTConsumer {
public:
  explicit DeprecatedFunctionConsumer(clang::CompilerInstance &CI)
      : Visitor(CI) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  DeprecatedFunctionVisitor Visitor;
};

class DeprecatedFunctionAction : public clang::PluginASTAction {
public:
  DeprecatedFunctionAction()
    { }

  virtual std::unique_ptr<clang::ASTConsumer>

  CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<DeprecatedFunctionConsumer>(CI);
  }
  bool ParseArgs(const clang::CompilerInstance &CI,
                  const std::vector<std::string> &args) override {
      return true;
    }
};

static clang::FrontendPluginRegistry::Add<DeprecatedFunctionAction>
    X("deprecated-function-warning", "Warns about deprecated functions usage");
