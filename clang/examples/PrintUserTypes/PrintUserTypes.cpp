#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

static const char *accessToString(AccessSpecifier AS) {
  switch (AS) {
  case AS_public:
    return "public";
  case AS_protected:
    return "protected";
  case AS_private:
    return "private";
  case AS_none:
    return "none";
  }
  return "none";
}

static AccessSpecifier getEffectiveAccess(const CXXRecordDecl *RD,
                                          AccessSpecifier AS) {
  if (AS != AS_none)
    return AS;
  return RD->isClass() ? AS_private : AS_public;
}

static std::string getSignature(const CXXMethodDecl *M) {
  std::string S;
  llvm::raw_string_ostream OS(S);

  OS << M->getReturnType().getAsString() << "(";
  for (unsigned I = 0; I < M->getNumParams(); ++I) {
    if (I)
      OS << ", ";
    OS << M->getParamDecl(I)->getType().getAsString();
  }
  OS << ")";

  return OS.str();
}

class PrintUserTypesVisitor
    : public RecursiveASTVisitor<PrintUserTypesVisitor> {
public:
  explicit PrintUserTypesVisitor(ASTContext &Context) : Context(Context) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *RD) {
    if (!RD->isThisDeclarationADefinition())
      return true;

    if (RD->isImplicit())
      return true;

    if (!RD->getIdentifier())
      return true;

    const SourceManager &SM = Context.getSourceManager();
    if (!SM.isWrittenInMainFile(SM.getSpellingLoc(RD->getLocation())))
      return true;

    printRecord(RD);
    return true;
  }

private:
  ASTContext &Context;

  void printRecord(const CXXRecordDecl *RD) {
    llvm::outs() << RD->getNameAsString();

    if (RD->getNumBases() > 0) {
      llvm::outs() << " -> ";
      bool First = true;
      for (const auto &Base : RD->bases()) {
        if (!First)
          llvm::outs() << ", ";
        First = false;
        llvm::outs() << Base.getType().getAsString();
      }
    }
    llvm::outs() << "\n";

    printFields(RD);
    llvm::outs() << "|\n";
    printMethods(RD);
    llvm::outs() << "\n";
  }

  void printFields(const CXXRecordDecl *RD) {
    llvm::outs() << "|_Fields\n";

    bool Empty = true;
    for (const FieldDecl *F : RD->fields()) {
      Empty = false;
      const AccessSpecifier AS = getEffectiveAccess(RD, F->getAccess());

      llvm::outs() << "| |_ " << F->getNameAsString() << " ("
                   << F->getType().getAsString() << "|"
                   << accessToString(AS) << ")\n";
    }

    if (Empty)
      llvm::outs() << "| |_ <none>\n";
  }

  void printMethods(const CXXRecordDecl *RD) {
    llvm::outs() << "|_Methods\n";

    bool Empty = true;
    for (const CXXMethodDecl *M : RD->methods()) {
      if (M->isImplicit())
        continue;

      Empty = false;
      const AccessSpecifier AS = getEffectiveAccess(RD, M->getAccess());

      llvm::outs() << "| |_ " << M->getNameAsString() << " ("
                   << getSignature(M) << "|" << accessToString(AS);

      if (M->isVirtual())
        llvm::outs() << "|virtual";
      if (M->isPureVirtual())
        llvm::outs() << "|pure";
      if (M->size_overridden_methods() > 0)
        llvm::outs() << "|override";

      llvm::outs() << ")\n";
    }

    if (Empty)
      llvm::outs() << "| |_ <none>\n";
  }
};

class PrintUserTypesConsumer : public ASTConsumer {
public:
  explicit PrintUserTypesConsumer(ASTContext &Context) : Visitor(Context) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  PrintUserTypesVisitor Visitor;
};

class PrintUserTypesAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<PrintUserTypesConsumer>(CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

} // namespace

static FrontendPluginRegistry::Add<PrintUserTypesAction>
    X("print-user-types", "print user-defined classes and structs");