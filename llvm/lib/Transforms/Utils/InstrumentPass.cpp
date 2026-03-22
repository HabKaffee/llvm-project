#include "llvm/Transforms/Utils/InstrumentPass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

using namespace llvm;

PreservedAnalyses InstrumentPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  if (F.isDeclaration())
    return PreservedAnalyses::all();

  Module *M = F.getParent();
  LLVMContext &Ctx = M->getContext();

  FunctionType *HookTy = FunctionType::get(Type::getVoidTy(Ctx), false);

  FunctionCallee StartFn =
      M->getOrInsertFunction("instrument_start", HookTy);
  FunctionCallee EndFn =
      M->getOrInsertFunction("instrument_end", HookTy);

  BasicBlock &Entry = F.getEntryBlock();
  IRBuilder<> EntryBuilder(&*Entry.getFirstInsertionPt());
  EntryBuilder.CreateCall(StartFn);

  for (BasicBlock &BB : F) {
    for (auto It = BB.begin(); It != BB.end();) {
      Instruction &I = *It++;
      if (isa<ReturnInst>(&I)) {
        IRBuilder<> RetBuilder(&I);
        RetBuilder.CreateCall(EndFn);
      }
    }
  }

  return PreservedAnalyses::none();
}