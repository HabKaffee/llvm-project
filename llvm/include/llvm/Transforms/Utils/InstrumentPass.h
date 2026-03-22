#ifndef LLVM_TRANSFORMS_UTILS_INSTRUMENTPASS_H
#define LLVM_TRANSFORMS_UTILS_INSTRUMENTPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class InstrumentPass : public PassInfoMixin<InstrumentPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif