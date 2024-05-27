#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

namespace {
    class AlwaysInlineNoConditionPass : public PassInfoMixin<AlwaysInlineNoConditionPass> {
    public:
        PreservedAnalyses run(Function &func, FunctionAnalysisManager &) {
            errs() << "Running pass on function: " << func.getName() << "\n";
            bool hasCondition = false;

            for (BasicBlock &basicBlock : func) {
                for (Instruction &inst : basicBlock) {
                    if (isa<ICmpInst>(&inst) || isa<SelectInst>(&inst)) {
                        if (auto *branchInst = dyn_cast<ICmpInst>(&inst)) {
                            errs() << "Found conditional branch in function: " << func.getName() << "\n";
                            hasCondition = true;
                            break;
                        } else if (isa<SelectInst>(&inst)) {
                            errs() << "Found switch instruction in function: " << func.getName() << "\n";
                            hasCondition = true;
                            break;
                        }
                    }
                }
                if (hasCondition) {
                    break;
                }
            }

            if (!hasCondition) {
                func.addFnAttr(Attribute::AlwaysInline);
                errs() << "Added always_inline to function: " << func.getName() << "\n";
            } else {
                errs() << "Did not add always_inline to function: " << func.getName() << " because it has conditions\n";
                func.addFnAttr(Attribute::NoInline);
            }

            return PreservedAnalyses::all();
        }
    };

}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
            LLVM_PLUGIN_API_VERSION, "AlwaysInlineNoConditionPass", LLVM_VERSION_STRING,
            [](PassBuilder &passBuilder) {
                passBuilder.registerPipelineParsingCallback(
                        [](StringRef name, FunctionPassManager &functionPassManager,
                           ArrayRef<PassBuilder::PipelineElement>) {
                            if (name == "always-inline-no-condition") {
                                functionPassManager.addPass(AlwaysInlineNoConditionPass());
                                return true;
                            }
                            return false;
                        });
            }
    };
}
