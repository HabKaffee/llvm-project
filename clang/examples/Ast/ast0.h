#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

class ImplicitCastVisitor : public RecursiveASTVisitor<ImplicitCastVisitor>
{
public:
    size_t GetCount() const{
        return Count;
    }

    bool VisitImplicitCastExpr(ImplicitCastExpr *expr){
        if (expr->getCastKind() == CK_FloatingCast){
            std::cout << "cast between floats (different size)" << std::endl;
            Count++;
        }
        if (expr->getCastKind() == CK_IntegralToFloating){
            std::cout << "integral -> floating" << std::endl;
            Count++;
        }
        if (expr->getCastKind() == CK_FloatingToIntegral){
            std::cout << "floating -> integral" << std::endl;
            Count++;
        }

        return true;
    }

private:
    size_t Count = 0;
};

class ImplicitCastConsumer : public ASTConsumer
{
public:
    void HandleTranslationUnit(ASTContext &Context) override
    {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        std::cout << Visitor.GetCount() << std::endl;
    }

private:
    ImplicitCastVisitor Visitor;
};

class ImplicitCastAction : public ASTFrontendAction
{
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &, StringRef) override
    {
        return std::make_unique<ImplicitCastConsumer>();
    }
};
