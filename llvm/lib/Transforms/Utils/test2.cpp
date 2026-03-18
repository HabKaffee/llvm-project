#include "llvm/Transforms/Utils/test2.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

PreservedAnalyses InstrumentFunctionsPass::run(Function &F, FunctionAnalysisManager &AM) {
    Module *M = F.getParent();
    LLVMContext &Ctx = M->getContext();

    FunctionCallee startFunc = M->getOrInsertFunction("instrument_start", Type::getVoidTy(Ctx));
    FunctionCallee endFunc = M->getOrInsertFunction("instrument_end", Type::getVoidTy(Ctx));

  
    if (F.isDeclaration()) return PreservedAnalyses::all();

    IRBuilder<> Builder(Ctx);

    Builder.SetInsertPoint(&F.getEntryBlock(), F.getEntryBlock().begin());
    Builder.CreateCall(startFunc);

    for (BasicBlock &BB : F) {
        if (isa<ReturnInst>(BB.getTerminator())) {
            Builder.SetInsertPoint(BB.getTerminator());
            Builder.CreateCall(endFunc);
        }
    }

    return PreservedAnalyses::all();
}
