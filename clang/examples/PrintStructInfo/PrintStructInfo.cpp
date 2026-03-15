//===--- PrintStructInfo.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception.
//
//===----------------------------------------------------------------------===//

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

// Преобразование спецификатора доступа в строку
std::string getAccessString(AccessSpecifier AS) {
  switch (AS) {
  case AS_public:    return "public";
  case AS_protected: return "protected";
  case AS_private:   return "private";
  default:           return "unknown";
  }
}

// Формирование строки с квалификаторами метода
std::string getMethodQualifiers(const CXXMethodDecl *Method) {
  std::string Quals;
  if (Method->isPureVirtual()) {
    Quals += "|virtual|pure";
  } else if (Method->size_overridden_methods() > 0) {
    Quals += "|override";
  } else if (Method->isVirtual() && !isa<CXXDestructorDecl>(Method)) {
    Quals += "|virtual";
  }
  return Quals;
}

class PrintStructVisitor : public RecursiveASTVisitor<PrintStructVisitor> {
public:
  explicit PrintStructVisitor(ASTContext &Ctx) : Context(Ctx) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *Record) {
    // Интересуют только определения, не неявные, и только из главного файла
    //llvm::outs() << "Visiting!!\n";
    if (!Record->isThisDeclarationADefinition() || Record->isImplicit())
      return true;
    if (!Context.getSourceManager().isWrittenInMainFile(Record->getLocation()))
      return true;

    // Вывод имени класса/структуры
    llvm::outs() << Record->getNameAsString();

    // Базовые классы
    if (Record->getNumBases() > 0) {
      llvm::outs() << " -> ";
      bool First = true;
      for (const auto &Base : Record->bases()) {
        if (!First)
          llvm::outs() << ", ";
        First = false;
        if (auto *BaseDecl = Base.getType()->getAsCXXRecordDecl())
          llvm::outs() << BaseDecl->getNameAsString();
        else
          llvm::outs() << Base.getType().getAsString();
      }
    }
    llvm::outs() << "\n";

    // Поля
    llvm::outs() << "|_Fields\n";
    for (auto *Field : Record->fields()) {
      llvm::outs() << "| |_ " << Field->getNameAsString() << " ("
                   << Field->getType().getAsString() << "|"
                   << getAccessString(Field->getAccess()) << ")\n";
    }

    // Методы (исключаем конструкторы, деструкторы, операторы преобразования)
    llvm::outs() << "|\n|_Methods\n";
    for (auto *Method : Record->methods()) {
      if (Method->isImplicit())
        continue;
      if (isa<CXXConstructorDecl>(Method) || isa<CXXDestructorDecl>(Method) ||
          isa<CXXConversionDecl>(Method))
        continue;

      std::string Access = getAccessString(Method->getAccess());
      std::string Quals = getMethodQualifiers(Method);
      std::string ReturnType = Method->getReturnType().getAsString();

      llvm::outs() << "| |_ " << Method->getNameAsString() << " ("
                   << ReturnType << "()|" << Access << Quals << ")\n";
    }

    llvm::outs() << "\n";
    return true;
  }

private:
  ASTContext &Context;
};

class PrintStructConsumer : public ASTConsumer {
public:
  explicit PrintStructConsumer(ASTContext &Ctx) : Visitor(Ctx) {}

  void HandleTranslationUnit(ASTContext &Ctx) override {
    //llvm::outs() << "HandleTranslationUnit!!1\n";
    Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }

private:
  PrintStructVisitor Visitor;
};

class PrintStructAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                  llvm::StringRef) override {
    //llvm::outs() << "CreateASTConsumer!!1\n";
    return std::make_unique<PrintStructConsumer>(CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &Args) override {
    // Можно добавить обработку аргументов, но пока не требуется
    return true;
  }

  // Действие будет выполнено после парсинга, но до основного действия
  //ActionType getActionType() override { return AddBeforeMainAction; }
};

} // namespace

static FrontendPluginRegistry::Add<PrintStructAction>
    X("print-struct-info", "Print information about struct/class: fields, methods, base classes");