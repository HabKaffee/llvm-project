#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

#include <memory>
#include <string>
#include <unordered_map>

using namespace clang;

namespace {

class CollectorVisitor : public RecursiveASTVisitor<CollectorVisitor> {
public:
    explicit CollectorVisitor(ASTContext &Context)
        : Context(Context) {}

    std::unordered_map<const NamedDecl*, std::string> RenameMap;

    bool VisitParmVarDecl(ParmVarDecl *P) {
        if (!isFromMainFile(P)) return true;
        if (P->getName().empty()) return true;

        RenameMap[P] = "param_" + P->getNameAsString();
        return true;
    }

    bool VisitVarDecl(VarDecl *V) {
        if (!isFromMainFile(V)) return true;
        if (V->getName().empty()) return true;

        if (isa<ParmVarDecl>(V)) return true;

        std::string oldName = V->getNameAsString();

        if (V->isLocalVarDecl()) {
            if (V->isStaticLocal())
                RenameMap[V] = "static_" + oldName;
            else
                RenameMap[V] = "local_" + oldName;
        } else if (V->hasGlobalStorage() && V->isFileVarDecl()) {
            RenameMap[V] = "global_" + oldName;
        }

        return true;
    }

private:
    ASTContext &Context;

    bool isFromMainFile(const Decl *D) const {
        return Context.getSourceManager().isWrittenInMainFile(
            Context.getSourceManager().getSpellingLoc(D->getLocation()));
    }
};

class RewriteVisitor : public RecursiveASTVisitor<RewriteVisitor> {
public:
    RewriteVisitor(ASTContext &Context,
                   Rewriter &R,
                   const std::unordered_map<const NamedDecl*, std::string> &Map)
        : Context(Context), R(R), RenameMap(Map) {}

    bool VisitParmVarDecl(ParmVarDecl *P) {
        rewriteDeclName(P);
        return true;
    }

    bool VisitVarDecl(VarDecl *V) {
        if (!isa<ParmVarDecl>(V))
            rewriteDeclName(V);
        return true;
    }

    bool VisitDeclRefExpr(DeclRefExpr *DRE) {
        const ValueDecl *VD = DRE->getDecl();
        auto it = RenameMap.find(VD);
        if (it == RenameMap.end()) return true;

        if (!isFromMainFile(DRE->getLocation())) return true;

        std::string oldName = VD->getNameAsString();
        if (oldName.empty()) return true;

        R.ReplaceText(DRE->getLocation(), oldName.size(), it->second);
        return true;
    }

private:
    ASTContext &Context;
    Rewriter &R;
    const std::unordered_map<const NamedDecl*, std::string> &RenameMap;

    void rewriteDeclName(NamedDecl *D) {
        auto it = RenameMap.find(D);
        if (it == RenameMap.end()) return;

        SourceLocation Loc = D->getLocation();
        if (!isFromMainFile(Loc)) return;

        std::string oldName = D->getNameAsString();
        if (oldName.empty()) return;

        R.ReplaceText(Loc, oldName.size(), it->second);
    }

    bool isFromMainFile(SourceLocation Loc) const {
        return Context.getSourceManager().isWrittenInMainFile(
            Context.getSourceManager().getSpellingLoc(Loc));
    }
};

class PrefixASTConsumer : public ASTConsumer {
public:
    explicit PrefixASTConsumer(CompilerInstance &CI)
        : Context(CI.getASTContext()) {
        R.setSourceMgr(Context.getSourceManager(), Context.getLangOpts());
    }

    void HandleTranslationUnit(ASTContext &Context) override {
        CollectorVisitor collector(Context);
        collector.TraverseDecl(Context.getTranslationUnitDecl());

        RewriteVisitor rewriter(Context, R, collector.RenameMap);
        rewriter.TraverseDecl(Context.getTranslationUnitDecl());

        R.getEditBuffer(Context.getSourceManager().getMainFileID()).write(llvm::outs());
    }

private:
    ASTContext &Context;
    Rewriter R;
};

class PrefixPluginAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   llvm::StringRef) override {
        return std::make_unique<PrefixASTConsumer>(CI);
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }
};

} // namespace

static FrontendPluginRegistry::Add<PrefixPluginAction>
X("prefix-plugin", "add prefixes to globals, locals, statics and params");