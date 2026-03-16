#ifndef LLVM_TRANSFORMS_UTILS_SIMPLEINLINER_H
#define LLVM_TRANSFORMS_UTILS_SIMPLEINLINER_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class SimpleInlinerPass : public PassInfoMixin<SimpleInlinerPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

} // namespace llvm

#endif