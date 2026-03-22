#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace {

    class PrefixHandler : public MatchFinder::MatchCallback {
    public:
        explicit PrefixHandler(Rewriter& R) : Rewrite(R) {}

        void run(const MatchFinder::MatchResult& Result) override {
            if (const auto* P = Result.Nodes.getNodeAs<ParmVarDecl>("param")) {
                processDecl(P, "param_", Result);
            }
            else if (const auto* V = Result.Nodes.getNodeAs<VarDecl>("global")) {
                processDecl(V, "global_", Result);
            }
            else if (const auto* V = Result.Nodes.getNodeAs<VarDecl>("static_local")) {
                processDecl(V, "static_", Result);
            }
            else if (const auto* V = Result.Nodes.getNodeAs<VarDecl>("local")) {
                processDecl(V, "local_", Result);
            }
            else if (const auto* Use = Result.Nodes.getNodeAs<DeclRefExpr>("use")) {
                processUse(Use, Result);
            }
        }

    private:
        std::map<const ValueDecl*, std::string> Renamed;
        Rewriter& Rewrite;

        void processDecl(const NamedDecl* D,
            const std::string& Prefix,
            const MatchFinder::MatchResult& Result) {

            std::string OldName = D->getNameAsString();
            if (OldName.empty()) return;

            const ValueDecl* VD = dyn_cast<ValueDecl>(D);
            if (!VD) return;

            if (Renamed.count(VD)) return;

            std::string NewName = Prefix + OldName;
            Renamed[VD] = NewName;

            SourceLocation Loc = D->getLocation();
            if (Loc.isInvalid()) return;

            Loc = Result.SourceManager->getSpellingLoc(Loc);
            if (!Result.SourceManager->isInMainFile(Loc)) return;

            unsigned Len = Lexer::MeasureTokenLength(
                Loc,
                *Result.SourceManager,
                Result.Context->getLangOpts()
            );

            Rewrite.ReplaceText(Loc, Len, NewName);
        }

        void processUse(const DeclRefExpr* Expr,
            const MatchFinder::MatchResult& Result) {

            const ValueDecl* D = Expr->getDecl();
            if (!D) return;

            auto It = Renamed.find(D);
            if (It == Renamed.end()) return;

            std::string OldName = D->getNameAsString();
            std::string NewName = It->second;

            SourceLocation Loc = Expr->getLocation();
            if (Loc.isInvalid()) return;

            Loc = Result.SourceManager->getSpellingLoc(Loc);
            if (!Result.SourceManager->isInMainFile(Loc)) return;

            unsigned Len = Lexer::MeasureTokenLength(
                Loc,
                *Result.SourceManager,
                Result.Context->getLangOpts()
            );

            Rewrite.ReplaceText(Loc, Len, NewName);
        }
    };

    class PrefixAction : public PluginASTAction {
    public:
        PrefixAction() : Handler(Rewrite) {}

    protected:
        std::unique_ptr<ASTConsumer> CreateASTConsumer(
            CompilerInstance& CI,
            StringRef) override {

            Rewrite.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());

            Finder.addMatcher(parmVarDecl().bind("param"), &Handler);

            Finder.addMatcher(
                varDecl(hasGlobalStorage(),
                    unless(hasAncestor(functionDecl())))
                .bind("global"),
                &Handler);

            Finder.addMatcher(
                varDecl(hasStaticStorageDuration(),
                    hasAncestor(functionDecl()))
                .bind("static_local"),
                &Handler);

            Finder.addMatcher(
                varDecl(hasAutomaticStorageDuration(),
                    unless(parmVarDecl()),
                    hasAncestor(functionDecl()))
                .bind("local"),
                &Handler);

            
            Finder.addMatcher(declRefExpr().bind("use"), &Handler);

            return Finder.newASTConsumer();
        }

        void EndSourceFileAction() override {
            Rewrite.getEditBuffer(
                Rewrite.getSourceMgr().getMainFileID()
            ).write(llvm::outs());
        }

        bool ParseArgs(const CompilerInstance&,
            const std::vector<std::string>&) override {
            return true;
        }

    private:
        Rewriter Rewrite;
        MatchFinder Finder;
        PrefixHandler Handler;
    };

} 

static FrontendPluginRegistry::Add<PrefixAction>
X("23cst6_Oreshkin_lab1", "add prefixes to variables (global_, local_, static_, param_)");