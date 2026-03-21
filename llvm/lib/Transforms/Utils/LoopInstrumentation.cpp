#include "llvm/Transforms/Utils/LoopInstrumentation.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/SmallPtrSet.h"

using namespace llvm;

PreservedAnalyses LoopInstrumentationPass::run(Function& F, FunctionAnalysisManager& FAM) {

    LoopInfo& LI = FAM.getResult<LoopAnalysis>(F);
    if (LI.empty())
        return PreservedAnalyses::all();

    Module* M = F.getParent();
    LLVMContext& Ctx = M->getContext();

    FunctionCallee loopStart =
        M->getOrInsertFunction("loop_start",
            FunctionType::get(Type::getVoidTy(Ctx), false));
    FunctionCallee loopEnd =
        M->getOrInsertFunction("loop_end",
            FunctionType::get(Type::getVoidTy(Ctx), false));

    for (Loop* L : LI) {
        instrumentLoop(L, loopStart, loopEnd);
    }
    return PreservedAnalyses::none();
}

void LoopInstrumentationPass::instrumentLoop(
    Loop* L,
    FunctionCallee loopStart,
    FunctionCallee loopEnd) {

    BasicBlock* Header = L->getHeader();

    if (!Header->empty()) {
        IRBuilder<> Builder(&*Header->getFirstInsertionPt());
        Builder.CreateCall(loopStart);
    }

    SmallVector<BasicBlock*, 4> ExitBlocks;
    L->getExitBlocks(ExitBlocks);
    SmallPtrSet<BasicBlock*, 4> UniqueExits(ExitBlocks.begin(), ExitBlocks.end());

    for (BasicBlock* Exit : UniqueExits) {
        if (Exit->empty())
            continue;
        IRBuilder<> Builder(&*Exit->getFirstInsertionPt());
        Builder.CreateCall(loopEnd);
    }

    for (Loop* SubLoop : L->getSubLoops()) {
        instrumentLoop(SubLoop, loopStart, loopEnd);
    }
}
