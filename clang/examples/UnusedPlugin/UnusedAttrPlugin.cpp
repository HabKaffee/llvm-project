#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/Attr.h"

using namespace clang;

class UnusedAttrVisitor : public RecursiveASTVisitor<UnusedAttrVisitor> {
public:
  explicit UnusedAttrVisitor(ASTContext *Context, DiagnosticsEngine &Diags)
      : Context(Context), Diags(Diags) {
    DiagID = Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                   "[[maybe_unused]] added to unused variable/parameter '%0'");
  }

  bool VisitFunctionDecl(FunctionDecl *FD) {
    for (auto *Param : FD->parameters()) {
      if (!Param->isUsed() && !Param->hasAttr<UnusedAttr>()) {
        Param->addAttr(UnusedAttr::CreateImplicit(*Context));
        Diags.Report(Param->getLocation(), DiagID) << Param->getNameAsString();
      }
    }
    return true;
  }

  bool VisitVarDecl(VarDecl *VD) {
    if (VD->isLocalVarDecl() && !VD->isUsed() && !VD->hasAttr<UnusedAttr>()) {
      VD->addAttr(UnusedAttr::CreateImplicit(*Context));
      Diags.Report(VD->getLocation(), DiagID) << VD->getNameAsString();
    }
    return true;
  }

private:
  ASTContext *Context;
  DiagnosticsEngine &Diags;
  unsigned DiagID;
};

class UnusedAttrConsumer : public ASTConsumer {
public:
  explicit UnusedAttrConsumer(ASTContext *Context, DiagnosticsEngine &Diags)
      : Visitor(Context, Diags) {}

  void HandleTranslationUnit(ASTContext &Ctx) override {
    Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }

private:
  UnusedAttrVisitor Visitor;
};

class UnusedAttrAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef) override {
    return std::make_unique<UnusedAttrConsumer>(&CI.getASTContext(),
                                                CI.getDiagnostics());
  }

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }
};

static FrontendPluginRegistry::Add<UnusedAttrAction>
    X("unused-attr", "Add [[maybe_unused]] to unused vars/params");
