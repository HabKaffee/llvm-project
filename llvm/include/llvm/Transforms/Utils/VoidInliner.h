#ifndef LLVM_TRANSFORMS_UTILS_VOIDINLINER_H
#define LLVM_TRANSFORMS_UTILS_VOIDINLINER_H

#include "llvm/IR/Analysis.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

class VoidInlinerPass : public PassInfoMixin<VoidInlinerPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif
