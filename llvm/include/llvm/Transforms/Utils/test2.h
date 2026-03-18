#ifndef LLVM_TRANSFORMS_UTILS_TEST2_H
#define LLVM_TRANSFORMS_UTILS_TEST2_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

class InstrumentFunctionsPass : public PassInfoMixin<InstrumentFunctionsPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

}

#endif 
