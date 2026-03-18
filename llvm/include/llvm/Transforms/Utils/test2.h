#ifndef LLVM_TRANSFORMS_UTILS_TEST2_H
#define LLVM_TRANSFORMS_UTILS_TEST2_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

class Test2Pass : public PassInfoMixin<Test2Pass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

}

#endif
