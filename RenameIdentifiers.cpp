
// Rewrite.overwriteChangedFiles();
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

class RenameVariableVisitor : public RecursiveASTVisitor<RenameVariableVisitor> {
public:
    RenameVariableVisitor(ASTContext *Context, Rewriter &R, const std::string &OldName, const std::string &NewName)
        : Context(Context), TheRewriter(R), OldName(OldName), NewName(NewName) {}

    bool VisitDeclRefExpr(DeclRefExpr *Expr) {
        if (Expr->getNameInfo().getAsString() == OldName) {
            TheRewriter.ReplaceText(Expr->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        return true;
    }

    bool VisitVarDecl(VarDecl *Declaration) {
        if (Declaration->getNameAsString() == OldName) {
            TheRewriter.ReplaceText(Declaration->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        if (Declaration->getType().getAsString() == OldName) {
            SourceLocation StartLoc = Declaration->getTypeSpecStartLoc();
            TheRewriter.ReplaceText(StartLoc, OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        return true;
    }
    
    bool VisitFieldDecl(FieldDecl *Declaration) {
        if (Declaration->getNameAsString() == OldName) {
            TheRewriter.ReplaceText(Declaration->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        if (Declaration->getType().getAsString() == OldName) {
            SourceLocation StartLoc = Declaration->getTypeSpecStartLoc();
            TheRewriter.ReplaceText(StartLoc, OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        return true;
    }
    
    bool VisitReturnStmt(ReturnStmt *Return) {
        if (CXXConstructExpr *Construct = dyn_cast<CXXConstructExpr>(Return->getRetValue())) {
            if (Construct->getConstructor()->getNameAsString() == OldName) {
                SourceLocation StartLoc = Construct->getBeginLoc();
                SourceLocation EndLoc = Construct->getEndLoc().getLocWithOffset(1); // Include the closing parenthesis
                std::string OldText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), 
                                                           TheRewriter.getSourceMgr(), TheRewriter.getLangOpts()).str();
                std::string NewText = OldText;
                size_t Pos = NewText.find(OldName);
                if (Pos != std::string::npos) {
                    NewText.replace(Pos, OldName.size(), NewName);
                    TheRewriter.ReplaceText(StartLoc, OldName.size(), NewText);
                    //TheRewriter.overwriteChangedFiles();
                }
            }
        }
        return true;
    }
    
    
    bool VisitFunctionDecl(FunctionDecl *Declaration) {
        if (Declaration->getNameAsString() == OldName) {
            TheRewriter.ReplaceText(Declaration->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        if (Declaration->getReturnType().getAsString() == OldName) {
            const auto &SourceMgr = TheRewriter.getSourceMgr();
            SourceLocation StartLoc = Declaration->getReturnTypeSourceRange().getBegin();
            TheRewriter.ReplaceText(StartLoc, OldName.size(), NewName);
        }
        return true;
    }
    
    
    bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
        if (Declaration->getNameAsString() == OldName) {
            TheRewriter.ReplaceText(Declaration->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        return true;
    }
    
    bool VisitParmVarDecl(ParmVarDecl *Declaration) {
        if (Declaration->getNameAsString() == OldName) {
            TheRewriter.ReplaceText(Declaration->getLocation(), OldName.size(), NewName);
            //TheRewriter.overwriteChangedFiles();
        }
        return true;
    }

private:
    ASTContext *Context;
    Rewriter &TheRewriter;
    std::string OldName;
    std::string NewName;
};

class RenameVariableConsumer : public ASTConsumer {
public:
    RenameVariableConsumer(ASTContext *Context, Rewriter &R, const std::string &OldName, const std::string &NewName)
        : Visitor(Context, R, OldName, NewName) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    RenameVariableVisitor Visitor;
};

class RenameVariableAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<RenameVariableConsumer>(&CI.getASTContext(), TheRewriter, OldName, NewName);
    }

    bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
        if (args.size() != 2) {
            llvm::errs() << "Expected exactly two arguments: <old-name> <new-name>\n";
            return false;
        }
        OldName = args[0];
        NewName = args[1];
        return true;
    }

    void EndSourceFileAction() override {
        //TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
        TheRewriter.overwriteChangedFiles();
    }

private:
    Rewriter TheRewriter;
    std::string OldName;
    std::string NewName;
};

static FrontendPluginRegistry::Add<RenameVariableAction>
    X(/*Name=*/"rename-id", /*Description=*/"Rename ids");


