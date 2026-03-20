#include "llvm/Transforms/Utils/LoopInstrumentation.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static void processLoop(Loop *L,
                        FunctionCallee StartFn,
                        FunctionCallee EndFn) {

  for (Loop *SubL : L->getSubLoops())
    processLoop(SubL, StartFn, EndFn);

  if (BasicBlock *Preheader = L->getLoopPreheader()) {
    IRBuilder<> B(Preheader->getTerminator());
    B.CreateCall(StartFn);
  }

  SmallVector<BasicBlock *, 8> ExitBlocks;
  L->getExitBlocks(ExitBlocks);

  for (BasicBlock *Exit : ExitBlocks) {
    IRBuilder<> B(Exit->getFirstNonPHI());
    B.CreateCall(EndFn);
  }
}

PreservedAnalyses
LoopInstrumentationPass::run(Function &F, FunctionAnalysisManager &AM) {

  if (F.isDeclaration())
    return PreservedAnalyses::all();

  auto &LI = AM.getResult<LoopAnalysis>(F);

  if (LI.empty())
    return PreservedAnalyses::all();

  Module *M = F.getParent();
  LLVMContext &Ctx = M->getContext();

  FunctionCallee StartFn =
      M->getOrInsertFunction("loop_start", Type::getVoidTy(Ctx));
  FunctionCallee EndFn =
      M->getOrInsertFunction("loop_end", Type::getVoidTy(Ctx));

  for (Loop *L : LI)
    processLoop(L, StartFn, EndFn);

  return PreservedAnalyses::none();
}
