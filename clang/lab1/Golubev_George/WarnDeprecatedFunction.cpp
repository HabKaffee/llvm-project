#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Pragma.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

using namespace clang;

template <unsigned N>
void emitWarningMessage(DiagnosticsEngine &DE, char const (&Message)[N],
                        FunctionDecl *FD) {
  unsigned const Id = DE.getCustomDiagID(DiagnosticsEngine::Warning, Message);
  DiagnosticBuilder const DB = DE.Report(FD->getLocation(), Id);
}

class WarnDeprecatedFunctionVisitor
    : public RecursiveASTVisitor<WarnDeprecatedFunctionVisitor> {
public:
  WarnDeprecatedFunctionVisitor(CompilerInstance &Instance)
      : Instance(Instance){};

  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (FD->getName().contains("deprecated")) {
      emitWarningMessage(Instance.getDiagnostics(), "Found deprecated function",
                         FD);
    }
    return true;
  }

private:
  CompilerInstance &Instance;
};

class WarnDeprecatedFunctionConsumer : public ASTConsumer {
public:
  explicit WarnDeprecatedFunctionConsumer(CompilerInstance &Instance)
      : Visitor(Instance){};

  virtual void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  WarnDeprecatedFunctionVisitor Visitor;
};

class WarnDeprecatedFunctionAction : public PluginASTAction {
public:
  virtual std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<WarnDeprecatedFunctionConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }
};

static FrontendPluginRegistry::Add<WarnDeprecatedFunctionAction>
    X("warn-deprecated-func",
      "Add warning when sees funtion with 'deprecated' in name");
