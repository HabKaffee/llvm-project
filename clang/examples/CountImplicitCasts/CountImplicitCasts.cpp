#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace clang;

namespace {

class CastVisitor : public RecursiveASTVisitor<CastVisitor> {
private:
    ASTContext *Context;
    std::vector<std::string> FunctionOrder;
    std::map<std::string, std::vector<std::string>> OrderMap;
    std::map<std::string, std::map<std::string, int>> Stats;
    std::string CurrentFunction = "<global>";

    std::string getNiceType(QualType QT) {
        QT = QT.getUnqualifiedType();
        PrintingPolicy PP(Context->getLangOpts());
        PP.SuppressTagKeyword = true;
        PP.SuppressScope = true;
        return QT.getAsString(PP);
    }

public:
    explicit CastVisitor(ASTContext *Ctx) : Context(Ctx) {}

    bool TraverseFunctionDecl(FunctionDecl *FD) {
        if (!FD || !FD->hasBody() || FD->isImplicit())
            return RecursiveASTVisitor::TraverseFunctionDecl(FD);

        CurrentFunction = FD->getNameAsString();

        if (std::find(FunctionOrder.begin(), FunctionOrder.end(), CurrentFunction) == FunctionOrder.end())
            FunctionOrder.push_back(CurrentFunction);

        bool Ret = RecursiveASTVisitor::TraverseFunctionDecl(FD);
        
        CurrentFunction = "<global>";
        return Ret;
    }

    bool VisitImplicitCastExpr(ImplicitCastExpr *ICE) {
        if (Context->getSourceManager().isInSystemHeader(ICE->getBeginLoc()))
            return true;

        if (ICE->getCastKind() == CK_LValueToRValue ||
            ICE->getCastKind() == CK_NoOp ||
            ICE->getCastKind() == CK_FunctionToPointerDecay)
            return true;

        std::string From = getNiceType(ICE->getSubExpr()->getType());
        std::string To = getNiceType(ICE->getType());

        if (From != To) {
            std::string Key = From + " -> " + To;
            
            if (Stats[CurrentFunction].find(Key) == Stats[CurrentFunction].end()) {
                OrderMap[CurrentFunction].push_back(Key);
                if (std::find(FunctionOrder.begin(), FunctionOrder.end(), CurrentFunction) == FunctionOrder.end()) {
                    FunctionOrder.push_back(CurrentFunction);
                }
            }
            Stats[CurrentFunction][Key]++;
        }
        return true;
    }

    void PrintStats() {
        for (const auto &Func : FunctionOrder) {
            auto it = Stats.find(Func);
            if (it == Stats.end() || it->second.empty())
                continue;

            llvm::outs() << "Function `" << Func << "`\n";

            for (const auto &Key : OrderMap[Func]) {
                llvm::outs() << "  " << Key << ": " << Stats[Func][Key] << "\n";
            }
            llvm::outs() << "\n";
        }
    }
};

class CastConsumer : public ASTConsumer {
    CastVisitor Visitor;
public:
    explicit CastConsumer(ASTContext *Ctx) : Visitor(Ctx) {}

    void HandleTranslationUnit(ASTContext &Ctx) override {
        Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
        Visitor.PrintStats();
    }
};

class CastAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        return std::make_unique<CastConsumer>(&CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &, const std::vector<std::string> &) override {
        return true;
    }

    ActionType getActionType() override {
        return AddBeforeMainAction;
    }
};

}

static FrontendPluginRegistry::Add<CastAction>
    X("count-implicit-casts", "Counts implicit casts per function");
