#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <string>

namespace {

    struct Lab2Pass : public llvm::PassInfoMixin<Lab2Pass> {

        llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &) {
            if (F.isDeclaration())
                return llvm::PreservedAnalyses::all();

            std::map<std::string, int> Stats;

            for (auto &BB : F) {
                for (auto &I : BB) {
                    std::string Name = I.getOpcodeName();
                    Stats[Name]++;
                }
            }

            llvm::errs() << "Function: " << F.getName() << "\n";

            // entry.first - operation's name
            // entry.second - operation's count
            for (auto &Entry : Stats) {
                llvm::errs() << Entry.first << ": " << Entry.second << "\n";
            }

            llvm::errs() << "\n";

            return llvm::PreservedAnalyses::all();
        }
    };

} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
            LLVM_PLUGIN_API_VERSION, "Lab2Pass", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](llvm::StringRef Name,
                           llvm::FunctionPassManager &FPM,
                           llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {

                            if (Name == "lab2-pass") {
                                FPM.addPass(Lab2Pass());
                                return true;
                            }
                            return false;
                        });
            }};
}
