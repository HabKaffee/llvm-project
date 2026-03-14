#include "llvm/Transforms/Utils/VoidInliner.h"

#include "llvm/IR/Analysis.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

PreservedAnalyses VoidInlinerPass::run(Function &F,
                                       FunctionAnalysisManager &AM) {
  std::vector<CallInst *> callsToInline;

  for (auto &&basicBlock : F) {
    for (auto &instruction : basicBlock) {
      if (auto *call = llvm::dyn_cast<CallInst>(&instruction)) {
        Function *callee = call->getCalledFunction();
        if (!callee || !callee->arg_empty() ||
            !callee->getReturnType()->isVoidTy() || callee->isDeclaration()) {
          continue;
        }

        callsToInline.push_back(call);
      }
    }
  }

  bool changed = false;
  InlineFunctionInfo IFI;
  for (auto *call : callsToInline) {
    if (InlineFunction(*call, IFI).isSuccess()) {
      changed = true;
    }
  }

  if (changed) {
    return PreservedAnalyses ::none();
  }
  return PreservedAnalyses::all();
}
