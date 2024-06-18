#include "WrapLoop.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

#define DEBUG_TYPE "wrap-loop"

bool WrapLoop::runOnLoop(Loop *L, Function *F) {

  Module *M = F->getParent();

  FunctionCallee LoopStart =
      M->getOrInsertFunction("loop_start", Type::getVoidTy(F->getContext()));
  FunctionCallee LoopEnd =
      M->getOrInsertFunction("loop_end", Type::getVoidTy(F->getContext()));

  BasicBlock *Preheader = L->getLoopPreheader();
  BasicBlock *ExitBlock = L->getExitBlock();

  if (!Preheader) {
    errs() << "Loop does not have a preheader.\n";
    return false;
  }

  IRBuilder<> Builder(Preheader->getTerminator());

  Builder.CreateCall(LoopStart);
  Builder.SetInsertPoint(ExitBlock, ExitBlock->getFirstInsertionPt());
  Builder.CreateCall(LoopEnd);
  return true;
}

PreservedAnalyses WrapLoop::run(llvm::Function &F,
                                llvm::FunctionAnalysisManager &AM) {
  LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

  for (Loop *L : LI) {
    runOnLoop(L, &F);
  }

  return PreservedAnalyses::all();
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getWrapLoopPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "wrap-loop", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "wrap-loop") {
                    FPM.addPass(WrapLoop());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getWrapLoopPluginInfo();
}
