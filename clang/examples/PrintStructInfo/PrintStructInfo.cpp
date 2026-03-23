#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>

using namespace clang;

namespace {

// ---------- УТИЛИТЫ ----------

static std::string accessToStr(AccessSpecifier AS, bool isStruct) {
  if (AS == AS_none)
    return isStruct ? "public" : "private";

  switch (AS) {
  case AS_public: return "public";
  case AS_private: return "private";
  case AS_protected: return "protected";
  default: return "none";
  }
}

static std::string buildMethodSignature(const CXXMethodDecl *M) {
  std::string result;
  llvm::raw_string_ostream os(result);

  os << M->getReturnType().getAsString() << "(";

  for (unsigned i = 0; i < M->getNumParams(); ++i) {
    if (i != 0)
      os << ", ";
    os << M->getParamDecl(i)->getType().getAsString();
  }

  os << ")";
  return os.str();
}

// ---------- VISITOR ----------

class StructVisitor : public RecursiveASTVisitor<StructVisitor> {
public:
  explicit StructVisitor(ASTContext &ctx) : Context(ctx) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *RD) {
    if (!shouldProcess(RD))
      return true;

    printClass(RD);
    return true;
  }

private:
  ASTContext &Context;

  bool shouldProcess(CXXRecordDecl *RD) {
    if (!RD->isThisDeclarationADefinition())
      return false;

    if (RD->isImplicit())
      return false;

    if (!RD->getIdentifier())
      return false;

    const SourceManager &SM = Context.getSourceManager();
    return SM.isWrittenInMainFile(SM.getSpellingLoc(RD->getLocation()));
  }

  void printClass(const CXXRecordDecl *RD) {
    llvm::outs() << RD->getNameAsString();

    printBases(RD);
    llvm::outs() << "\n";

    printFields(RD);
    llvm::outs() << "|\n";
    printMethods(RD);

    llvm::outs() << "\n";
  }

  void printBases(const CXXRecordDecl *RD) {
    if (RD->getNumBases() == 0)
      return;

    llvm::outs() << " -> ";

    bool first = true;
    for (const auto &Base : RD->bases()) {
      if (!first)
        llvm::outs() << ", ";

      first = false;
      llvm::outs() << Base.getType().getAsString();
    }
  }

  void printFields(const CXXRecordDecl *RD) {
    llvm::outs() << "|_Fields\n";

    bool empty = true;

    for (const FieldDecl *F : RD->fields()) {
      empty = false;

      std::string access = accessToStr(F->getAccess(), RD->isStruct());

      llvm::outs() << "| |_ "
                   << F->getNameAsString()
                   << " ("
                   << F->getType().getAsString()
                   << "|"
                   << access
                   << ")\n";
    }

    if (empty)
      llvm::outs() << "| |_ <none>\n";
  }

  void printMethods(const CXXRecordDecl *RD) {
    llvm::outs() << "|_Methods\n";

    bool empty = true;

    for (const CXXMethodDecl *M : RD->methods()) {
      if (skipMethod(M))
        continue;

      empty = false;

      std::string access = accessToStr(M->getAccess(), RD->isStruct());

      llvm::outs() << "| |_ "
                   << M->getNameAsString()
                   << " ("
                   << buildMethodSignature(M)
                   << "|"
                   << access;

      appendMethodQualifiers(M);

      llvm::outs() << ")\n";
    }

    if (empty)
      llvm::outs() << "| |_ <none>\n";
  }

  bool skipMethod(const CXXMethodDecl *M) {
    if (M->isImplicit())
      return true;

    if (isa<CXXConstructorDecl>(M) ||
        isa<CXXDestructorDecl>(M) ||
        isa<CXXConversionDecl>(M))
      return true;

    return false;
  }

  void appendMethodQualifiers(const CXXMethodDecl *M) {
    if (M->isVirtual())
      llvm::outs() << "|virtual";

    if (M->isPureVirtual())
      llvm::outs() << "|pure";

    if (M->size_overridden_methods() > 0)
      llvm::outs() << "|override";
  }
};

// ---------- CONSUMER ----------

class StructConsumer : public ASTConsumer {
public:
  explicit StructConsumer(ASTContext &ctx) : Visitor(ctx) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  StructVisitor Visitor;
};

// ---------- ACTION ----------

class StructAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<StructConsumer>(CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

} // namespace

static FrontendPluginRegistry::Add<StructAction>
    X("print-struct-info", "print info about user-defined types");