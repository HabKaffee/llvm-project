#ifndef LLVM_TRANSFORMS_UTILS_INSTRUMENTPASS_H
#define LLVM_TRANSFORMS_UTILS_INSTRUMENTPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

// Наследуемся от PassInfoMixin для поддержки New Pass Manager
class InstrumentPass : public PassInfoMixin<InstrumentPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_INSTRUMENTPASS_H