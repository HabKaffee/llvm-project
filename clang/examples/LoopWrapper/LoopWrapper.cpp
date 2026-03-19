#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {

struct LoopWrapperPass : public PassInfoMixin<LoopWrapperPass> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);

    Module &M = *F.getParent();
    FunctionType *VoidFnType = FunctionType::get(Type::getVoidTy(M.getContext()), false);

    FunctionCallee LoopStart = M.getOrInsertFunction("loop_start", VoidFnType);
    FunctionCallee LoopEnd = M.getOrInsertFunction("loop_end", VoidFnType);

    for (Loop *L : LI) {
        processLoop(L, LoopStart, LoopEnd);
    }

    return PreservedAnalyses::none();
}

private:
  void processLoop(Loop *L, FunctionCallee LoopStart, FunctionCallee LoopEnd) {
    for (Loop *SubLoop : L->getSubLoops()) {
        processLoop(SubLoop, LoopStart, LoopEnd);
    }

    BasicBlock *PreHeader = L->getLoopPreheader();
    if(PreHeader == nullptr) {
      return; // default handling - advice from AI        
    }
    IRBuilder<> Builder(PreHeader->getTerminator());
    Builder.CreateCall(LoopStart);


    SmallVector<BasicBlock *, 4> ExitBlocks;
    L->getExitBlocks(ExitBlocks);

    for (BasicBlock *Exit : ExitBlocks) {
        IRBuilder<> Builder(&Exit->front());
        Builder.CreateCall(LoopEnd);
    }
  }
};

} // namespace

// регистрация плагина
llvm::PassPluginLibraryInfo getLoopWrapperPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopWrapper", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-wrapper") {
                    FPM.addPass(LoopWrapperPass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopWrapperPluginInfo();
}