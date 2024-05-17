#include <iostream>
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

class PrintClassAttrsVisitor
  : public RecursiveASTVisitor<PrintClassAttrsVisitor> {
public:
  explicit PrintClassAttrsVisitor(ASTContext *context, CompilerInstance &Compiler)
	: context(context), Compiler(Compiler) {}
  virtual bool VisitCXXRecordDecl(CXXRecordDecl *Decl) {
    llvm::outs() << Decl->getNameAsString() << '\n';
    for (const auto& field: Decl->fields()) {
      const auto& name = field->getNameAsString();
      llvm::outs() << "  |_" << name << '\n';
    }
    return true;
  }
private:
  ASTContext *context;
  CompilerInstance &Compiler;
};


class PrintClassAttrsConsumer : public ASTConsumer {
public:
  PrintClassAttrsConsumer(CompilerInstance &Compiler)
	: Compiler(Compiler), Visitor(&Compiler.getASTContext(), Compiler) {}

  virtual void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  CompilerInstance &Compiler;
  PrintClassAttrsVisitor Visitor;
};

class PrintClassAttrsAction : public PluginASTAction {
public:
  virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(
    CompilerInstance &Compiler, llvm::StringRef InFile) override {
    return std::make_unique<PrintClassAttrsConsumer>(Compiler);
  }
   virtual bool ParseArgs(const CompilerInstance &Compiler, const std::vector<std::string> &args) override {
    return true;
  }
  virtual PluginASTAction::ActionType getActionType() override {
    return PluginASTAction::AddAfterMainAction;
  }
};

static FrontendPluginRegistry::Add<PrintClassAttrsAction> PrintClassAttrs("PrintClassAttrs", "Print class names and his members.");
