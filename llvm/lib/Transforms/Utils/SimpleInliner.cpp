#include "llvm/Transforms/Utils/SimpleInliner.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static void inlineCall(CallInst *CI) {
  Function *Caller = CI->getFunction();
  Function *Callee = CI->getCalledFunction();

  ValueToValueMapTy VMap;
  SmallVector<ReturnInst *, 4> Returns;

  BasicBlock *CallBB = CI->getParent();
  BasicBlock *AfterCallBB = CallBB->splitBasicBlock(CI, "after.inline");

  CallBB->getTerminator()->eraseFromParent();

  CloneFunctionInto(Caller, Callee, VMap,
                    CloneFunctionChangeType::LocalChangesOnly, Returns);

  BasicBlock *ClonedEntry = cast<BasicBlock>(VMap[&Callee->getEntryBlock()]);
  IRBuilder<>(CallBB).CreateBr(ClonedEntry);

  for (ReturnInst *RI : Returns) {
    BasicBlock *RetBB = RI->getParent();
    RI->eraseFromParent();
    IRBuilder<>(RetBB).CreateBr(AfterCallBB);
  }

  CI->eraseFromParent();
}

PreservedAnalyses SimpleInlinerPass::run(Module &M, ModuleAnalysisManager &) {
  bool Changed = false;

  for (Function &F : M) {
    if (F.isDeclaration())
      continue;

    SmallVector<CallInst *, 8> ToInline;

    for (BasicBlock &BB : F)
      for (Instruction &I : BB)
        if (auto *CI = dyn_cast<CallInst>(&I)) {
          Function *Callee = CI->getCalledFunction();
          if (!Callee || Callee->isDeclaration())
            continue;
          if (!Callee->getReturnType()->isVoidTy())
            continue;
          if (Callee->arg_size() != 0)
            continue;
          if (Callee == &F)
            continue;
          ToInline.push_back(CI);
        }

    for (CallInst *CI : ToInline) {
      inlineCall(CI);
      Changed = true;
    }
  }

  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}