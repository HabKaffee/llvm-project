#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/Expr.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <string>

using namespace clang;

class CastVisitor : public RecursiveASTVisitor<CastVisitor> {
public:
    explicit CastVisitor(ASTContext *Context)
        : Context(Context) {}

    bool VisitFunctionDecl(FunctionDecl *Func) {
        if (!Func->hasBody())
            return true;

        currentFunction = Func->getNameAsString();
        casts.clear();

        TraverseStmt(Func->getBody());

        llvm::outs() << "Function `" << currentFunction << "`\n";
        for (auto &entry : casts) {
            llvm::outs() << entry.first << ": " << entry.second << "\n";
        }

        return true;
    }

    bool VisitImplicitCastExpr(ImplicitCastExpr *Cast) {
        auto kind = Cast->getCastKind();

        // мусор
        if (kind == CK_LValueToRValue ||
            kind == CK_NoOp ||
            kind == CK_FunctionToPointerDecay)
            return true;

        QualType fromType = Cast->getSubExpr()->getType();
        QualType toType = Cast->getType();

        // игнор одинаковых типов хз откуда оно берется вообще
        if (fromType == toType)
            return true;

        std::string key =
            fromType.getAsString() + " -> " + toType.getAsString();

        casts[key]++;
        return true;
    }

private:
    ASTContext *Context;
    std::string currentFunction;
    std::map<std::string, int> casts;
};

class CastConsumer : public ASTConsumer {
public:
    explicit CastConsumer(ASTContext *Context)
        : Visitor(Context) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    CastVisitor Visitor;
};

class CastAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, llvm::StringRef) override {
        return std::make_unique<CastConsumer>(&CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }
};

static FrontendPluginRegistry::Add<CastAction>
    X("count-casts", "Count implicit casts");