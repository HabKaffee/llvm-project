#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/SmallPtrSet.h"

using namespace clang;

namespace {

class UsageCollector : public RecursiveASTVisitor<UsageCollector> {
public:
  llvm::SmallPtrSet<const ValueDecl *, 16> UsedDecls;

  bool VisitDeclRefExpr(DeclRefExpr *DRE) {
    if (auto *VD = dyn_cast<ValueDecl>(DRE->getDecl()->getCanonicalDecl()))
        UsedDecls.insert(VD);
    
    return true;
  }
};

class MaybeUnusedVisitor : public RecursiveASTVisitor<MaybeUnusedVisitor> {
  DiagnosticsEngine &Diags;
  unsigned DiagID;

public:
  MaybeUnusedVisitor(ASTContext &Ctx, DiagnosticsEngine &D)
      : Diags(D) {
    DiagID = Diags.getCustomDiagID(
        DiagnosticsEngine::Warning,
        "variable or parameter '%0' is unused; consider adding [[maybe_unused]]");
  }

  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (!FD->hasBody())
      return true;

    UsageCollector Collector;
    Collector.TraverseStmt(FD->getBody());

    for (ParmVarDecl *Param : FD->parameters()) {
      checkDecl(Param, Collector);
    }

    checkLocals(FD->getBody(), Collector);

    return true;
  }

private:
  bool hasMaybeUnused(const VarDecl *VD) {
    return VD->hasAttr<UnusedAttr>();
  }

  void checkDecl(const VarDecl *VD, const UsageCollector &Collector) {
    if (VD->isImplicit())
      return;
    if (hasMaybeUnused(VD))
      return;
    if (Collector.UsedDecls.count(VD->getCanonicalDecl()))
      return;

    SourceLocation Loc = VD->getBeginLoc();
    auto DB = Diags.Report(Loc, DiagID);
    DB << VD->getName();
    DB << FixItHint::CreateInsertion(Loc, "[[maybe_unused]] ");
  }

  void checkLocals(Stmt *S, const UsageCollector &Collector) {
    if (!S) return;
    if (auto *DS = dyn_cast<DeclStmt>(S)) {
      for (Decl *D : DS->decls()) {
        if (auto *VD = dyn_cast<VarDecl>(D)) {
          checkDecl(VD, Collector);
        }
      }
    }
    for (Stmt *Child : S->children()) {
      checkLocals(Child, Collector);
    }
  }
};

class MaybeUnusedConsumer : public ASTConsumer {
  CompilerInstance &CI;

public:
  MaybeUnusedConsumer(CompilerInstance &CI) : CI(CI) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    MaybeUnusedVisitor Visitor(Context, CI.getDiagnostics());
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class MaybeUnusedAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<MaybeUnusedConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }

  PluginASTAction::ActionType getActionType() override {
    return AddAfterMainAction;
  }
};

} // namespace

static FrontendPluginRegistry::Add<MaybeUnusedAction>
    X("maybe-unused-attr", "Mark unused variables and parameters with [[maybe_unused]]");