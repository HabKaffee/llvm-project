#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"
#include <map>

using namespace clang;

namespace {

class CastVisitor : public RecursiveASTVisitor<CastVisitor> {
public:
    explicit CastVisitor(ASTContext *Ctx) 
        : Context(Ctx), SM(Ctx->getSourceManager()) {}

    bool VisitImplicitCastExpr(ImplicitCastExpr *Cast) {
        if (!Cast) return true;
        
        // Пропускаем касты в системных заголовках
        if (SM.isInSystemHeader(Cast->getExprLoc()))
            return true;
        
        // Пропускаем LValueToRValue (не интересные нам)
        if (Cast->getCastKind() == CK_LValueToRValue)
            return true;
        
        Expr *Sub = Cast->getSubExpr();
        if (!Sub) return true;
        
        QualType Src = Sub->getType().getCanonicalType().getUnqualifiedType();
        QualType Dst = Cast->getType().getCanonicalType().getUnqualifiedType();
        
        if (Src != Dst) {
            std::string SrcName = getTypeName(Src);
            std::string DstName = getTypeName(Dst);
            if (!SrcName.empty() && !DstName.empty()) {
                casts[CurrentFunction][SrcName + " -> " + DstName]++;
            }
        }
        
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *FD) {
        if (FD->hasBody()) {
            CurrentFunction = FD->getNameAsString();
        }
        return true;
    }

    void printResults() {
        if (casts.empty()) {
            llvm::outs() << "No implicit casts found.\n";
            return;
        }
        
        llvm::outs() << "\n";
        for (const auto &func : casts) {
            llvm::outs() << "Function " << func.first << "\n";
            for (const auto &cast : func.second) {
                llvm::outs() << "  " << cast.first << ": " << cast.second << "\n";
            }
            llvm::outs() << "\n";
        }
    }

private:
    ASTContext *Context;
    SourceManager &SM;
    std::map<std::string, std::map<std::string, int>> casts;
    std::string CurrentFunction;

    std::string getTypeName(QualType T) {
        if (T->isSpecificBuiltinType(BuiltinType::Int))
            return "int";
        if (T->isSpecificBuiltinType(BuiltinType::Float))
            return "float";
        if (T->isSpecificBuiltinType(BuiltinType::Double))
            return "double";
        return "";
    }
};

class CastConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Ctx) override {
        CastVisitor Visitor(&Ctx);
        Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
        Visitor.printResults();
    }
};

class CastAction : public PluginASTAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                    StringRef InFile) override {
        return std::make_unique<CastConsumer>();
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }

    PluginASTAction::ActionType getActionType() override {
        return AddBeforeMainAction;
    }
};

}

static FrontendPluginRegistry::Add<CastAction>
    X("cast-counter", "Counts implicit casts in functions");
