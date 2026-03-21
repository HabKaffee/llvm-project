#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

enum class VirtualKind { None, Virtual, Override, Pure };

static std::string accessToString(AccessSpecifier AS) {
  switch (AS) {
  case AS_public:    return "public";
  case AS_protected: return "protected";
  case AS_private:   return "private";
  default:           return "public";
  }
}

static std::string methodSignature(const CXXMethodDecl *MD, PrintingPolicy PP) {
  PP.SuppressScope = true;
  PP.SuppressUnwrittenScope = true;
  llvm::SmallString<256> Buf;
  llvm::raw_svector_ostream OS(Buf);
  MD->getReturnType().print(OS, PP);
  OS << "(";
  bool First = true;
  for (const ParmVarDecl *P : MD->parameters()) {
    if (!First)
      OS << ", ";
    First = false;
    P->getType().print(OS, PP);
  }
  OS << ")";
  if (MD->isConst())
    OS << " const";
  if (MD->isVolatile())
    OS << " volatile";
  switch (MD->getRefQualifier()) {
  case RQ_LValue: OS << " &";  break;
  case RQ_RValue: OS << " &&"; break;
  case RQ_None:                break;
  }
  return std::string(Buf.str());
}

static void printMethodQualifiers(llvm::raw_ostream &OS, const CXXMethodDecl *MD) {
  if (MD->isStatic())
    OS << "|static";

  VirtualKind VK = VirtualKind::None;
  if (MD->isPureVirtual())
    VK = VirtualKind::Pure;
  else if (MD->hasAttr<OverrideAttr>())
    VK = VirtualKind::Override;
  else if (MD->isVirtual())
    VK = VirtualKind::Virtual;

  switch (VK) {
  case VirtualKind::Pure:     OS << "|virtual|pure"; break;
  case VirtualKind::Override: OS << "|override";     break;
  case VirtualKind::Virtual:  OS << "|virtual";      break;
  case VirtualKind::None:                            break;
  }
}

static void printRecord(CXXRecordDecl *RD, ASTContext &Ctx, llvm::raw_ostream &OS) {
  OS << RD->getNameAsString();
  if (RD->getNumBases() > 0) {
    OS << " -> ";
    bool First = true;
    for (const CXXBaseSpecifier &Base : RD->bases()) {
      if (!First)
        OS << ", ";
      First = false;
      QualType BT = Base.getType();
      if (const CXXRecordDecl *BR = BT->getAsCXXRecordDecl())
        OS << BR->getNameAsString();
      else
        BT.print(OS, Ctx.getPrintingPolicy());
    }
  }
  OS << "\n";

  PrintingPolicy PP = Ctx.getPrintingPolicy();
  PP.SuppressScope = true;
  PP.PrintAsCanonical = true;

  OS << "|_Fields\n";
  for (FieldDecl *FD : RD->fields()) {
    OS << "| |_ " << FD->getNameAsString() << " (";
    FD->getType().print(OS, PP);
    OS << "|" << accessToString(FD->getAccess()) << ")\n";
  }

  OS << "|\n|_Methods\n";
  for (const CXXMethodDecl *MD : RD->methods()) {
    if (MD->isImplicit())
      continue;
    if (isa<CXXConstructorDecl>(MD) || isa<CXXDestructorDecl>(MD))
      continue;
    OS << "| |_ " << MD->getNameAsString() << " (" << methodSignature(MD, PP)
       << "|" << accessToString(MD->getAccess());
    printMethodQualifiers(OS, MD);
    OS << ")\n";
  }
  OS << "\n";
}

class StructInfoVisitor : public RecursiveASTVisitor<StructInfoVisitor> {
  CompilerInstance &CI;
  llvm::raw_ostream &OS;

public:
  StructInfoVisitor(CompilerInstance &CI, llvm::raw_ostream &OS)
      : CI(CI), OS(OS) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *RD) {
    if (!RD->isThisDeclarationADefinition())  return true;
    if (RD->isDependentContext())             return true;
    if (RD->isLambda())                       return true;
    if (RD->isAnonymousStructOrUnion())       return true;
    if (RD->getDescribedClassTemplate())      return true;

    SourceLocation Loc = RD->getLocation();
    if (Loc.isInvalid() || CI.getSourceManager().isInSystemHeader(Loc))
      return true;

    printRecord(RD, CI.getASTContext(), OS);
    return true;
  }
};

class StructInfoConsumer : public ASTConsumer {
  CompilerInstance &CI;

public:
  explicit StructInfoConsumer(CompilerInstance &CI) : CI(CI) {}

  void HandleTranslationUnit(ASTContext &Ctx) override {
    std::string Output;
    llvm::raw_string_ostream OS(Output);
    StructInfoVisitor V(CI, OS);
    V.TraverseDecl(Ctx.getTranslationUnitDecl());
    llvm::outs() << Output;
  }
};

class StructInfoPluginAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<StructInfoConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

}

static FrontendPluginRegistry::Add<StructInfoPluginAction>
    X("struct-info", "print struct/class fields, methods, and bases");
