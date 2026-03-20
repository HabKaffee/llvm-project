#include "llvm/Transforms/SimpleInliner/SimpleInliner.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

PreservedAnalyses SimpleInlinerPass::run(Function &F, FunctionAnalysisManager &AM) {
  bool Changed = false;
  std::vector<CallBase *> CallsToInline;

  for (auto &BB : F) {
    for (auto &I : BB) {
      if (auto *CB = dyn_cast<CallBase>(&I)) {
        Function *CalledF = CB->getCalledFunction();
        if (CalledF && !CalledF->isDeclaration() &&
            CalledF->getReturnType()->isVoidTy() &&
            CalledF->arg_empty()) {
          CallsToInline.push_back(CB);
        }
      }
    }
  }

  for (auto *CB : CallsToInline) {
    InlineFunctionInfo IFI;
    auto Result = llvm::InlineFunction(*CB, IFI, false, nullptr, true, nullptr, nullptr);
    if (Result.isSuccess()) {
      Changed = true;
    }
  }

  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
