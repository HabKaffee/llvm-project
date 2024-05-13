#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

class PrintClassFieldsVisitor : public RecursiveASTVisitor<PrintClassFieldsVisitor> {
public:
    explicit PrintClassFieldsVisitor(ASTContext *Context) : Context(Context) {}

    bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
        if (Declaration && Declaration->isThisDeclarationADefinition()) {
            llvm::outs() << "Class name: " << Declaration->getNameAsString() << "\n";
            llvm::outs() << "Fields:\n";
            for (const auto &Field : Declaration->fields()) {
                llvm::outs() << "└" <<Field->getNameAsString() << "\n";
            }
            llvm::outs() << "\n";
        }
        return true;
    }

private:
    ASTContext *Context;
};

class PrintClassFieldsConsumer : public ASTConsumer {
public:
    explicit PrintClassFieldsConsumer(ASTContext *Context) : Visitor(Context) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    PrintClassFieldsVisitor Visitor;
};

class PrintClassFieldsAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        return std::make_unique<PrintClassFieldsConsumer>(&CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
        return true;
    }
};

static FrontendPluginRegistry::Add<PrintClassFieldsAction>
    X(/*Name=*/"print-class-fields", /*Description=*/"Prints class names and fields");

