#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ParentMapContext.h" //for getParents

#include <map>
#include <string>

using namespace clang;

namespace {

class ImplicitCastCounterVisitor
    : public RecursiveASTVisitor<ImplicitCastCounterVisitor> {
public:
  explicit ImplicitCastCounterVisitor(ASTContext *Context) : Context(Context) {}

  bool VisitImplicitCastExpr(ImplicitCastExpr *Cast) {
    if (Context->getSourceManager().isInSystemHeader(Cast->getExprLoc()))
        return true;

    if (Cast->getCastKind() == CK_FunctionToPointerDecay) return true; // преобразовние функции в значение отсекаем

    std::string SrcTypeStr = Cast->getSubExpr()->getType().getUnqualifiedType().getAsString();
    std::string DstTypeStr = Cast->getType().getUnqualifiedType().getAsString();
    std::string CastLocStr = getImplicitCastLoc(Cast);

    if (SrcTypeStr == DstTypeStr) return true;
    
    ImplicitCastInfo[CastLocStr][SrcTypeStr + " -> " + DstTypeStr]++;
    return true;
  }

  const auto& getImplicitCastInfo() const {
    return ImplicitCastInfo;
  }

private:
  ASTContext *Context;
  std::map<std::string, std::map<std::string, int>> ImplicitCastInfo;
  std::string getImplicitCastLoc(const ImplicitCastExpr *Cast) {
    auto Parents = Context->getParents(*Cast);

    while (!Parents.empty()) {
        const auto &ParentNode = Parents[0];
        auto ptr = ParentNode.template get<FunctionDecl>();
        if(ptr != nullptr) {
            return "Function `" + ptr->getNameAsString() + "`";
        }

        Parents = Context->getParents(ParentNode);
    }

    return "Not a Function";
  }
};

class ImplicitCastCounterConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &Context) override {
    ImplicitCastCounterVisitor visitor(&Context);
    visitor.TraverseDecl(Context.getTranslationUnitDecl());
    auto ImplicitCastInfo = visitor.getImplicitCastInfo();
    for(const auto& funcName: ImplicitCastInfo) {
        llvm::outs() << funcName.first << "\n";
        for(const auto& CastName: funcName.second) {
            llvm::outs() << CastName.first << ": " << CastName.second << "\n";
        }
        llvm::outs() << "\n";
    }
  }
};

class ImplicitCastCounterAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<ImplicitCastCounterConsumer>();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }

  PluginASTAction::ActionType getActionType() override {
    return AddBeforeMainAction;
  }
};

} // namespace

static FrontendPluginRegistry::Add<ImplicitCastCounterAction>
    X("implicit-cast-counter", "Counts implicit casts");