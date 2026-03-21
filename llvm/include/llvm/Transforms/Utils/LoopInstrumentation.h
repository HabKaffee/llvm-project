#ifndef LLVM_TRANSFORMS_HELLONEW_LOOPINSTRUMENTATION_H
#define LLVM_TRANSFORMS_HELLONEW_LOOPINSTRUMENTATION_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;
class Loop;
class FunctionCallee;

class LoopInstrumentationPass : public PassInfoMixin<LoopInstrumentationPass> {
public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& FAM);
private:
    void instrumentLoop(Loop* L, FunctionCallee loopStart, FunctionCallee loopEnd);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_HELLONEW_LOOPINSTRUMENTATION_H
