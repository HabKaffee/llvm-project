#ifndef LLVM_TRANSFORMS_UTILS_LOOPINSTRUMENTATION_H
#define LLVM_TRANSFORMS_UTILS_LOOPINSTRUMENTATION_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

class LoopInstrumentationPass : public PassInfoMixin<LoopInstrumentationPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif
