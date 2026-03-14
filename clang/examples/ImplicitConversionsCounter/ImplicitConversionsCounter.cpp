#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Expr.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

#include "llvm/Support/raw_ostream.h"

#include <map>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

using namespace clang;

namespace {

template <typename InputIterator, typename T>
InputIterator customFind(InputIterator first, InputIterator last, const T& value) {
    while (first != last) {
        if ((*first).first == value.first) {
            return first;
        }
        ++first;
    }
    return last;
}

class ImplicitCastVisitor
    : public RecursiveASTVisitor<ImplicitCastVisitor> {

    ASTContext *Context;
    const FunctionDecl *CurrentFunction = nullptr;

    std::map<std::string, std::map<std::string, unsigned>> Stats;
    std::vector<std::pair<std::string, std::deque<std::string>>> StatsOrder;

public:

    explicit ImplicitCastVisitor(ASTContext *Context)
        : Context(Context) {}

    bool VisitFunctionDecl(FunctionDecl *FD) {

	if (!FD)
            return true;

    	auto &SM = Context->getSourceManager();
    	SourceLocation Loc = FD->getLocation();

	if (Loc.isInvalid())
            return true;

	SourceLocation ExpansionLoc = SM.getExpansionLoc(Loc);

    	if (!SM.isWrittenInMainFile(ExpansionLoc))
            return true;

    	if (!FD->hasBody())
            return true;

        CurrentFunction = FD;
	StatsOrder.push_back(std::make_pair(CurrentFunction->getNameAsString(), std::deque<std::string>()));

        return true;
    }

    bool VisitImplicitCastExpr(ImplicitCastExpr *ICE) {

	auto &SM = Context->getSourceManager();
	SourceLocation Loc = ICE->getExprLoc();

	if (!SM.isWrittenInMainFile(SM.getExpansionLoc(Loc)))
    	    return true;

	CastKind kind = ICE->getCastKind();

    	if (kind == CK_NoOp ||
	    kind == CK_LValueToRValue ||
            kind == CK_FunctionToPointerDecay ||
            kind == CK_DerivedToBase ||
            kind == CK_UncheckedDerivedToBase)
            return true;

        QualType From = ICE->getSubExpr()->getType();
        QualType To = ICE->getType();

	if (From.getCanonicalType() == To.getCanonicalType())
	    return true;

	if (From->isFunctionType() || To->isFunctionType())
            return true;

        std::string FromStr = From.getAsString();
        std::string ToStr = To.getAsString();

        std::string Key = FromStr + " -> " + ToStr;

	std::string FuncName =
	    CurrentFunction ? CurrentFunction->getNameAsString() : "<global>";

	Stats[FuncName][Key]++;
	if (Stats[FuncName][Key] == 1)
	{
	    auto globalSpaceTemplate = std::make_pair("<global>", std::deque<std::string>());
	    if (StatsOrder.empty())
	    {
		StatsOrder.push_back(globalSpaceTemplate);
		StatsOrder.back().second.push_front(Key);
	    }
	    else if (FuncName == "<global>")
	    {
		const auto found_pos = customFind(StatsOrder.begin(), StatsOrder.end(), globalSpaceTemplate);
		if (found_pos != StatsOrder.end())
		{
		    auto index = std::distance(StatsOrder.begin(), found_pos);
		    StatsOrder[index].second.push_front(Key);
		}
	    }
	    else
		StatsOrder.back().second.push_front(Key);
	}

        return true;
    }

    void PrintStats() {

        for (const auto &Func : StatsOrder) {

            llvm::outs() << "Function `" << Func.first << "`\n";

            for (const auto &Conv : Func.second) {

                llvm::outs()
                    << Conv
                    << ": "
                    << Stats[Func.first][Conv]
                    << "\n";
            }

            llvm::outs() << "\n";
        }
    }
};

class ImplicitCastConsumer : public ASTConsumer {

    ImplicitCastVisitor Visitor;

public:

    explicit ImplicitCastConsumer(ASTContext *Context)
        : Visitor(Context) {}

    void HandleTranslationUnit(ASTContext &Context) override {

	Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        Visitor.PrintStats();
    }
};

class ImplicitCastAction : public PluginASTAction {

protected:

    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {

        return std::make_unique<ImplicitCastConsumer>(
            &CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {

        return true;
    }
};

}

static FrontendPluginRegistry::Add<ImplicitCastAction>
X("count-implicit-conversions",
  "count implicit conversions per function");
