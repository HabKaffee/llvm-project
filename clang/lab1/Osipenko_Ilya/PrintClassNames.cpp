#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {
class ClassNamesPrinterConsumer : public ASTConsumer {
public:
  bool HandleTopLevelDecl(DeclGroupRef DeclarationGroup) override {
    for (auto Declaration : DeclarationGroup) {
      auto *CRD = dyn_cast<CXXRecordDecl>(Declaration);
      if (CRD && CRD->isThisDeclarationADefinition()) {
        llvm::errs() << CRD->getQualifiedNameAsString() << "\n";
        PrintFieldsOfClass(CRD);
      }

      auto *CTD = dyn_cast<ClassTemplateDecl>(Declaration);
      if (CTD) {
        llvm::errs() << CTD->getQualifiedNameAsString() << "\n";
        PrintFieldsOfClass(CTD->getTemplatedDecl());
      }
    }
    return true;
  }

private:
  void PrintFieldsOfClass(const CXXRecordDecl *CRD) {
    for (const auto *Field : CRD->fields()) {
      llvm::errs() << "└" << Field->getName() << "\n";
    }
    llvm::errs() << "\n";
  }
};

class ClassNamesPrinterAction : public PluginASTAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<ClassNamesPrinterConsumer>();
  }
  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }
};
}

static FrontendPluginRegistry::Add<ClassNamesPrinterAction>
    X("class-names-printer", "Print class names");
