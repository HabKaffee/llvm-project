#include "llvm/Transforms/Utils/InstrumentStartEnd.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"

using namespace llvm;

PreservedAnalyses InstrumentStartEnd::run(Function &F, FunctionAnalysisManager &AM) {
  // skip to avoid recursion
  StringRef Name = F.getName();
  if (Name == "instrument_start" || Name == "instrument_end")
    return PreservedAnalyses::all();

  Module *M = F.getParent();
  LLVMContext &Ctx = M->getContext();

  FunctionType *VoidFnTy = FunctionType::get(Type::getVoidTy(Ctx), false);
  FunctionCallee StartFn = M->getOrInsertFunction("instrument_start", VoidFnTy);
  FunctionCallee EndFn = M->getOrInsertFunction("instrument_end", VoidFnTy);

  // insert instrument_start()
  IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());
  Builder.CreateCall(StartFn);

  SmallVector<ReturnInst *, 8> Returns;
  for (BasicBlock &BB : F)
    for (Instruction &I : BB)
      if (auto *RI = dyn_cast<ReturnInst>(&I))
        Returns.push_back(RI);

  // insert instrument_end()
  for (ReturnInst *RI : Returns) {
    Builder.SetInsertPoint(RI);
    Builder.CreateCall(EndFn);
  }

  return PreservedAnalyses::none();
}
