#ifndef LLVM_TRANSFORMS_UTILS_LOOPFINDER_H
#define LLVM_TRANSFORMS_UTILS_LOOPFINDER_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class LoopFinderPass : public PassInfoMixin<LoopFinderPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif