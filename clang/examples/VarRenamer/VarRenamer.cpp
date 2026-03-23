#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
//#includee "clang/Rewrite/Core/RewriteBuffer.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/raw_ostream.h"
#include <map>


using namespace clang;

namespace {
    
    class VarRenamerVisitor: public RecursiveASTVisitor<VarRenamerVisitor>{
        private:
            ASTContext *Context;
            Rewriter &Rew;
            SourceManager &SM;
            std::map<const NamedDecl*, std::string> RenamedDecls;
            bool Quiet;
        public:
            VarRenamerVisitor(ASTContext *Ctx, Rewriter &R, SourceManager &SrcMgr, bool Q)
                :Context(Ctx), Rew(R), SM(SrcMgr), Quiet(Q){}

            bool VisitVarDecl(VarDecl *VD){
                if(!SM.isInMainFile(VD->getLocation()))
                    return true;
                
                std::string Prefix;

                if(VD->isStaticLocal()){
                    Prefix = "static_";
                }

                else if (!VD->isLocalVarDecl() && VD->hasGlobalStorage() && !VD->isStaticLocal()){
                    Prefix = "global_";
                }
                else if(VD->isLocalVarDecl() && !VD->isStaticLocal()){
                    Prefix = "local_";
                }
                
                if (!Prefix.empty()){
                    SourceLocation Loc = VD->getLocation();
                    Rew.InsertText(Loc, Prefix, /*InsetrAfter=*/false);
                    std::string NewName = Prefix + VD->getNameAsString();
                    RenamedDecls[VD] = NewName;
                    if(!Quiet){
                    llvm::outs()<<"Renamed: "<< VD->getNameAsString()
                                <<" -> " << Prefix << VD->getNameAsString()<<"/n";
                    }
                }

                return true;
            }

            bool VisitFunctionDecl(FunctionDecl *FD){
                if (!SM.isInMainFile(FD->getLocation()))
                    return true;
                
                llvm::outs() << "Visiting function: " << FD->getNameAsString() << "\n"; 
                for (ParmVarDecl *PD : FD->parameters()){
                    SourceLocation Loc = PD->getLocation();
                    if(!Loc.isValid() || !SM.isInMainFile(Loc))
                        continue;

                    if(RenamedDecls.find(PD) != RenamedDecls.end())
                        continue;

                    Rew.InsertText(Loc, "param_", /*InsetrAfter=*/false);
                    std::string NewName = "param_" + PD->getNameAsString();
                    RenamedDecls[PD] = NewName;
                    if(!Quiet){
                    llvm::outs()<<"Renamed: "<< PD->getNameAsString()
                                <<" -> " << "param_" << PD->getNameAsString()<<"/n";
                    }
                }
    
                    return true;
            }
            
            bool VisitDeclRefExpr(DeclRefExpr *DRE){
                if (!SM.isInMainFile(DRE->getLocation()))
                    return true;
                
                if (const NamedDecl *ND = DRE->getDecl()){//? ==
                    auto it = RenamedDecls.find(ND);
                    if(it != RenamedDecls.end()){
                        SourceLocation Loc = DRE->getLocation();
                        //Change text from Loc to the end of origin name to new name
                        unsigned OrigLen = ND->getName().size();

                        Rew.ReplaceText(SourceRange(Loc, Loc.getLocWithOffset(OrigLen - 1)), it->second);
                    }
                }
                return true;
        }
    };

    class VarRenamerConsumer : public ASTConsumer{
        private:
            Rewriter Rew; //?
            CompilerInstance &CI;
            bool Quiet;

        public:
            VarRenamerConsumer(CompilerInstance &Compiler, bool Q) : CI(Compiler), Quiet(Q){
                Rew.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
                
            }

            void HandleTranslationUnit(ASTContext &Context) override {
                SourceManager &SM = CI.getSourceManager();

                VarRenamerVisitor Visitor(&Context, Rew, SM, Quiet);

                Visitor.TraverseDecl(Context.getTranslationUnitDecl());

                Rew.getEditBuffer(SM.getMainFileID()).write(llvm::outs());

            }
        };
        
        class VarRenamerAction : public PluginASTAction {
            bool Quiet = false;
            public:
                bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override{
                    for (const auto &arg : args){
                        if (arg == "-quiet")
                            Quiet = true;
                    }
                    return true;
                }
                std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override{
                    return std::make_unique<VarRenamerConsumer>(CI, Quiet);
                }
            protected:

                PluginASTAction::ActionType getActionType() override{
                    return AddAfterMainAction;
                }
        };

}

static FrontendPluginRegistry::Add<VarRenamerAction>
X("var-renamer", "Rename variables with prefixes: static_, local_, global_, param_");
