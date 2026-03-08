#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

#include <memory>
#include <string>

using namespace clang;

namespace {

class RewriteVisitor : public RecursiveASTVisitor<RewriteVisitor> {
public:
    RewriteVisitor(ASTContext &Context, Rewriter &R)
        : Context(Context), R(R) {}

    bool VisitParmVarDecl(ParmVarDecl *P) {
        // Обрабатываем объявление параметра функции.
        rewriteDeclName(P);
        return true;
    }

    bool VisitVarDecl(VarDecl *V) {
        // Параметры приходят сюда как VarDecl, но они уже обработаны выше.
        if (isa<ParmVarDecl>(V))
            return true;

        // Обрабатываем обычные локальные/глобальные/static переменные.
        rewriteDeclName(V);
        return true;
    }

    bool VisitDeclRefExpr(DeclRefExpr *DRE) {
        // Переписываем использования идентификаторов.
        if (!isFromMainFile(DRE->getLocation()))
            return true;

        // Для использования переменной вычисляем целевое имя с префиксом.
        const ValueDecl *VD = DRE->getDecl();
        std::string newName = getPrefixedName(VD);
        if (newName.empty())
            return true;

        std::string oldName = VD->getNameAsString();
        if (oldName.empty())
            return true;

        R.ReplaceText(DRE->getLocation(), oldName.size(), newName);
        return true;
    }

private:
    ASTContext &Context;
    Rewriter &R;

    void rewriteDeclName(NamedDecl *D) {
        // Не трогаем объявления не из основного файла.
        if (!isFromMainFile(D))
            return;

        std::string oldName = D->getNameAsString();
        if (oldName.empty())
            return;

        // Работаем только с сущностями, которые являются ValueDecl.
        const auto *VD = dyn_cast<ValueDecl>(D);
        if (!VD)
            return;

        // Вычисляем новое имя по типу объявления.
        std::string newName = getPrefixedName(VD);
        if (newName.empty())
            return;

        // Переписываем идентификатор в месте объявления.
        R.ReplaceText(D->getLocation(), oldName.size(), newName);
    }

    bool isFromMainFile(const Decl *D) const {
        // Вариант для Decl: берем location и делегируем в перегрузку ниже.
        return isFromMainFile(D->getLocation());
    }

    bool isFromMainFile(SourceLocation Loc) const {
        // Проверяем, что location принадлежит именно главному .cpp-файлу.
        return Context.getSourceManager().isWrittenInMainFile(
            Context.getSourceManager().getSpellingLoc(Loc));
    }

    static std::string getPrefixedName(const ValueDecl *D) {
        // Возвращаем итоговое имя с нужным префиксом:
        // param_ / local_ / static_ / global_.
        if (!D || D->getName().empty())
            return "";

        if (const auto *P = dyn_cast<ParmVarDecl>(D))
            return "param_" + P->getNameAsString();

        const auto *V = dyn_cast<VarDecl>(D);
        if (!V)
            return "";

        const std::string oldName = V->getNameAsString();
        if (V->isLocalVarDecl())
            return (V->isStaticLocal() ? "static_" : "local_") + oldName;
        if (V->hasGlobalStorage() && V->isFileVarDecl())
            return "global_" + oldName;

        return "";
    }
};

class PrefixASTConsumer : public ASTConsumer {
public:
    explicit PrefixASTConsumer(CompilerInstance &CI)
        : Context(CI.getASTContext()) {
        R.setSourceMgr(Context.getSourceManager(), Context.getLangOpts());
    }

    void HandleTranslationUnit(ASTContext &Context) override {
        RewriteVisitor rewriter(Context, R);
        rewriter.TraverseDecl(Context.getTranslationUnitDecl());

        // Выводим переписанный основной файл в stdout.
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

// инициализация плагина
static FrontendPluginRegistry::Add<PrefixPluginAction>
X("prefix-plugin", "add prefixes to globals, locals, statics and params");
