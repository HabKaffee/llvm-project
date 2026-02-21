#include "clang/AST/Attr.h"
#include "clang/AST/Attrs.inc"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"


using namespace clang;

namespace {
class MaybeUnusedVisitor : public RecursiveASTVisitor<MaybeUnusedVisitor> {
public:
    explicit MaybeUnusedVisitor(ASTContext&) {}

        bool VisitVarDecl(VarDecl* vd) {
            handleVarDecl(vd);
            return true;
        }

private:
    void handleVarDecl(VarDecl* vd) {
        auto toMarkUnused = [] (VarDecl* vd) -> bool {
            return vd && !vd->hasAttr<UnusedAttr>() && !vd->isUsed(false);
        };

        if(toMarkUnused(vd)) {
            vd->addAttr(UnusedAttr::CreateImplicit(
                vd->getASTContext(),
                vd->getBeginLoc()
            ));
        }
    }
};

class MaybeUnusedConsumer : public ASTConsumer {
public:
    explicit MaybeUnusedConsumer(ASTContext& ctx):
        myVisitor(ctx) {}

    void HandleTranslationUnit(ASTContext& ctx) override {
        myVisitor.TraverseDecl(ctx.getTranslationUnitDecl());

        // for debugging
        // ctx.getTranslationUnitDecl()->dump();
    }

private:
    MaybeUnusedVisitor myVisitor;
};

class MaybeUnusedAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance& ci,
        llvm::StringRef) override {
            return std::make_unique<MaybeUnusedConsumer>(ci.getASTContext());
    }

    bool ParseArgs(const CompilerInstance&, const std::vector<std::string>&) override {
        return true;
    }
 };

} // namespace

static FrontendPluginRegistry::Add<MaybeUnusedAction>
X("maybe-unused", "mark unused variables with [[maybe_unused]]");
