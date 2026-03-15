#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"       // для ConstantInt
#include "llvm/IR/PassManager.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class MultiplyOptPass : public PassInfoMixin<MultiplyOptPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        bool Changed = false;

        for (auto &BB : F) {
            for (auto InstIt = BB.begin(); InstIt != BB.end(); ) {
                Instruction &I = *InstIt++;
                if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
                    if (BO->getOpcode() == Instruction::Mul) {
                        Value *Op0 = BO->getOperand(0);
                        Value *Op1 = BO->getOperand(1);

                        if (!Op0->getType()->isIntegerTy() || !Op1->getType()->isIntegerTy())
                            continue;

                        ConstantInt *CInt = nullptr;
                        Value *OtherOp = nullptr;
                        if ((CInt = dyn_cast<ConstantInt>(Op0)) && CInt->getValue().isPowerOf2()) {
                            OtherOp = Op1;
                        } else if ((CInt = dyn_cast<ConstantInt>(Op1)) && CInt->getValue().isPowerOf2()) {
                            OtherOp = Op0;
                        } else {
                            continue; // ни один операнд не является степенью двойки
                        }

                        unsigned ShiftAmount = CInt->getValue().logBase2();

                        ConstantInt *ShiftConst = ConstantInt::get(Type::getInt32Ty(F.getContext()), ShiftAmount);

                        BinaryOperator *Shl = BinaryOperator::CreateShl(OtherOp, ShiftConst, "shl", BO);

                        BO->replaceAllUsesWith(Shl);

                        BO->eraseFromParent();

                        Changed = true;

                    }
                }
            }
        }

        return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
};

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MultiplyOptPass", "v0.1",
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "multiply-opt") {
                        FPM.addPass(MultiplyOptPass());
                        return true;
                    }
                    return false;
                });
        }};
}
