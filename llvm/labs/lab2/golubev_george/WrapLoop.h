#ifndef LLVM_WRAP_LOOP_H
#define LLVM_WRAP_LOOP_H

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

struct WrapLoop : public llvm::PassInfoMixin<WrapLoop> {
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);

  bool runOnLoop(llvm::Loop *L, llvm::Function *F);
  static bool isRequired() { return true; }
};

#endif
