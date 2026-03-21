#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class StructVisitor : public RecursiveASTVisitor<StructVisitor> {
public:
  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    // Игнорируем forward declarations и неявные классы компилятора
    if (!Declaration->isThisDeclarationADefinition() || Declaration->isImplicit() || 
        (!Declaration->isStruct() && !Declaration->isClass())) {
      return true;
    }

    // Имя и базовые классы
    llvm::outs() << Declaration->getNameAsString();
    if (Declaration->getNumBases() > 0) {
      llvm::outs() << " -> ";
      bool first = true;
      for (const auto &Base : Declaration->bases()) {
        if (!first) llvm::outs() << ", ";
        llvm::outs() << Base.getType().getAsString();
        first = false;
      }
    }
    llvm::outs() << "\n";

    // Поля
    llvm::outs() << "|_Fields\n";
    for (auto *Field : Declaration->fields()) {
      
      PrintingPolicy PP(Declaration->getASTContext().getLangOpts());
      PP.Bool = true; // печатать bool вместо _Bool

      std::string access = getAccessString(Field->getAccess());
      llvm::outs() << "| |_ " << Field->getNameAsString() << " ("
                   << Field->getType().getAsString(PP) << "|" << access << ")\n";
    }
    llvm::outs() << "|\n";

    // Методы
    llvm::outs() << "|_Methods\n";
    for (auto *Method : Declaration->methods()) {
      // Игнорируем конструкторы, деструкторы и неявные методы
      if (Method->isImplicit() || isa<CXXConstructorDecl>(Method) || isa<CXXDestructorDecl>(Method)) {
        continue;
      }

      std::string access = getAccessString(Method->getAccess());
      std::string attributes = "";

      if (Method->isVirtual() && Method->size_overridden_methods() == 0) attributes += "|virtual";
      if (Method->isPureVirtual()) attributes += "|pure virtual";
      if (Method->size_overridden_methods() > 0) attributes += "|override";

      llvm::outs() << "| |_ " << Method->getNameAsString() << " ("
                   << Method->getReturnType().getAsString() << "()|" << access
                   << attributes << ")\n";
    }

    return true;
  }

private:
  std::string getAccessString(AccessSpecifier AS) {
    switch (AS) {
      case AS_public: return "public";
      case AS_protected: return "protected";
      case AS_private: return "private";
      case AS_none: return "none";
    }
    return "unknown";
  }
};

class PrintStructInfoConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  StructVisitor Visitor;
};

class PrintStructInfoAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<PrintStructInfoConsumer>();
  }
  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }
};

} // namespace

// Регистрация плагина
static FrontendPluginRegistry::Add<PrintStructInfoAction>
    X("print-struct-info", "Prints information about structs/classes");