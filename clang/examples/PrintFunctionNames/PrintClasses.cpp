#include "clang/AST/AST.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class ClassVisitor : public RecursiveASTVisitor<ClassVisitor> {
public:

  bool VisitCXXRecordDecl(CXXRecordDecl *decl) {

    if (!decl->isThisDeclarationADefinition())
      return true;

    if (!decl->getIdentifier())
      return true;

    llvm::outs() << decl->getNameAsString();

    if (decl->getNumBases() > 0) {
      for (auto &base : decl->bases()) {
        llvm::outs() << " -> " << base.getType().getAsString();
      }
    }

    llvm::outs() << "\n";

    llvm::outs() << "|_Fields\n";

    for (auto field : decl->fields()) {
      llvm::outs()
          << "| |_ "
          << field->getNameAsString()
          << " ("
          << field->getType().getAsString()
          << ")\n";
    }

    llvm::outs() << "|_Methods\n";

    for (auto method : decl->methods()) {

      llvm::outs()
          << "| |_ "
          << method->getNameAsString()
          << " ("
          << method->getType().getAsString()
          << ")";

      if (method->isVirtual())
        llvm::outs() << "|virtual";

      if (method->isPure())
        llvm::outs() << "|pure";

      if (method->size_overridden_methods() > 0)
        llvm::outs() << "|override";

      llvm::outs() << "\n";
    }

    llvm::outs() << "\n";

    return true;
  }
};

class ClassConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &context) override {
    ClassVisitor visitor;
    visitor.TraverseDecl(context.getTranslationUnitDecl());
  }
};

class ClassAction : public PluginASTAction {

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(
      CompilerInstance &CI,
      llvm::StringRef) override {

    return std::make_unique<ClassConsumer>();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }
};

}

static FrontendPluginRegistry::Add<ClassAction>
X("print-classes", "print class info");
