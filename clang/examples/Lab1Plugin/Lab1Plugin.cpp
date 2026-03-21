#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/FrontendAction.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * This plugin finds all defined functions
 * and counts direct function calls
 */

namespace {

    class Lab1Visitor : public clang::RecursiveASTVisitor<Lab1Visitor> {

    private:
        // vector of funcs names (string)
        std::vector<std::string> DefinedFunctions;
        // name and number of calls
        std::map<std::string, int> CallStats;


    public:
        bool VisitFunctionDecl(clang::FunctionDecl *FD) {
            // if no functions (safety check null pointer)
            if (FD == nullptr) {
                return true;
            }
            // if just func declaration (no body)
            if (!FD->hasBody()) {
                return true;
            }

            const std::string Name = FD->getNameAsString();
            DefinedFunctions.push_back(Name);
            llvm::outs() << "function: " << Name << "\n";
            return true;
        }

        bool VisitCallExpr(clang::CallExpr *Call) {
            // if no call (safety check null pointer)
            if (Call == nullptr) {
                return true;
            }

            // direct call (not pointer or etc)
            const clang::FunctionDecl *DirectCallee = Call->getDirectCallee();
            // we work only with direct calls
            if (DirectCallee == nullptr) {
                return true;
            }

            const std::string CalleeName = DirectCallee->getNameAsString();
            // just counter
            ++CallStats[CalleeName];
            llvm::outs() << "call: " << CalleeName << "\n";
            return true;
        }

        void PrintSummary() const {
            llvm::outs() << "\nsummary:\n";
            for (const auto &FunctionName : DefinedFunctions) {
                auto It = CallStats.find(FunctionName);
                int Count = 0;

                if (It != CallStats.end()) {
                    Count = It->second;
                }

                llvm::outs() << FunctionName << " called " << Count << " time(s)\n";
            }
        }

    };

    class Lab1ASTConsumer : public clang::ASTConsumer {
    public:
        void HandleTranslationUnit(clang::ASTContext &Context) override {
            Lab1Visitor Visitor;
            // getTranslationUnitDecl() is a root of ast
            // visit the whole ast and doing smth
            Visitor.TraverseDecl(Context.getTranslationUnitDecl());
            // print stats
            Visitor.PrintSummary();
        }
    };

    class Lab1PluginAction : public clang::PluginASTAction {
    protected:
        // we dont use CompilerInstance -> we dont write CI name
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                clang::CompilerInstance &,
                llvm::StringRef) override {

            return std::make_unique<Lab1ASTConsumer>();
        }

        bool ParseArgs(const clang::CompilerInstance &,
                       const std::vector<std::string> &Args) override {
            for (const auto &Arg : Args) {
                if (Arg == "help") {
                    llvm::errs() << "Lab1Plugin usage:\n";
                    llvm::errs() << "  -plugin-arg-lab1_plugin help\n";
                }
            }
            return true;
        }

        clang::PluginASTAction::ActionType getActionType() override {
            return clang::PluginASTAction::AddAfterMainAction;
        }
    };

} // namespace

static clang::FrontendPluginRegistry::Add<Lab1PluginAction>
        X("lab1_plugin", "print functions, calls and call statistics");
