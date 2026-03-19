#include "llvm/Transforms/Utils/LoopFinder.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// recursive
static void processLoop(Loop *L, FunctionCallee loopStart, FunctionCallee loopEnd) {
    BasicBlock *Header = L->getHeader();

    errs() << "  Loop header: " << Header->getName() << "\n";

    // LOOP START
    Instruction *InsertPt = &*Header->getFirstInsertionPt();
    IRBuilder<> BuilderStart(InsertPt);
    BuilderStart.CreateCall(loopStart);

    // LOOP END
    for (BasicBlock *BB : L->getBlocks()) {
        for (Instruction &I : *BB) {
            if (auto *Br = dyn_cast<BranchInst>(&I)) {
                for (unsigned i = 0; i < Br->getNumSuccessors(); i++) {
                    BasicBlock *Succ = Br->getSuccessor(i);

                    if (!L->contains(Succ)) {
                        BasicBlock *ExitBB = Succ;

                        Instruction *InsertPt = &*ExitBB->getFirstInsertionPt();
                        IRBuilder<> BuilderEnd(InsertPt);
                        BuilderEnd.CreateCall(loopEnd);
                    }
                }
            }
        }
    }

    // inner loops
    for (Loop *SubLoop : L->getSubLoops()) {
        processLoop(SubLoop, loopStart, loopEnd);
    }
}

PreservedAnalyses LoopFinderPass::run(Function &F, FunctionAnalysisManager &AM) {
    if (F.isDeclaration())
        return PreservedAnalyses::all();

    errs() << "Function: " << F.getName() << "\n";

    auto &LI = AM.getResult<LoopAnalysis>(F);

    Module *M = F.getParent();
    LLVMContext &Ctx = M->getContext();

    // forward decl
    FunctionCallee loopStart = M->getOrInsertFunction(
        "loop_start", FunctionType::get(Type::getVoidTy(Ctx), false));

    FunctionCallee loopEnd = M->getOrInsertFunction(
        "loop_end", FunctionType::get(Type::getVoidTy(Ctx), false));

    // process loop
    for (Loop *L : LI) {
        processLoop(L, loopStart, loopEnd);
    }

    return PreservedAnalyses::none();
}