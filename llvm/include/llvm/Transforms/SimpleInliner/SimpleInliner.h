#ifndef LLVM_TRANSFORMS_SIMPLEINLINER_H
#define LLVM_TRANSFORMS_SIMPLEINLINER_H

#include "llvm/IR/PassManager.h"

namespace llvm {
class SimpleInlinerPass : public PassInfoMixin<SimpleInlinerPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // namespace llvm

#endif
