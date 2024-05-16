#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace {

class AlwaysInlineHandler : public MatchFinder::MatchCallback {
public:
  AlwaysInlineHandler(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  void run(const MatchFinder::MatchResult &Result) override {
    const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
    if (Func && Func->hasBody()) {
      llvm::errs() << "Processing function: " << Func->getNameAsString() << "\n";
      if (!hasCondition(Func)) {
        SourceLocation StartLoc = Func->getBeginLoc();
        Rewrite.InsertText(StartLoc, " __attribute__((always_inline))", true, true);
      }
    }
  }

private:
  bool hasCondition(const FunctionDecl *Func) {
    if (const Stmt *Body = Func->getBody()) {
      for (const Stmt *S : Body->children()) {
        if (isa<IfStmt>(S) || isa<SwitchStmt>(S) || isa<ConditionalOperator>(S)) {
          return true;
        }
      }
    }
    return false;
  }

  Rewriter &Rewrite;
};

class AlwaysInlineAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    RewriterForCI.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    Finder.addMatcher(functionDecl(isDefinition()).bind("func"), &Handler);
    return Finder.newASTConsumer();
  }

  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }

  void EndSourceFileAction() override {
    RewriterForCI.getEditBuffer(RewriterForCI.getSourceMgr().getMainFileID()).write(llvm::outs());
  }

private:
  Rewriter RewriterForCI;
  AlwaysInlineHandler Handler{RewriterForCI};
  MatchFinder Finder;
};

}

static FrontendPluginRegistry::Add<AlwaysInlineAction>
X("always-inline-plugin", "Add always_inline attribute to functions without conditions");

