#include "llvm/Transforms/Utils/test2.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;

PreservedAnalyses Test2Pass::run(Function &F, FunctionAnalysisManager &AM) {

  Module *M = F.getParent();
  LLVMContext &Ctx = M->getContext();

  // объявляем функции
  FunctionCallee startFunc =
      M->getOrInsertFunction("instrument_start", FunctionType::get(Type::getVoidTy(Ctx), false));

  FunctionCallee endFunc =
      M->getOrInsertFunction("instrument_end", FunctionType::get(Type::getVoidTy(Ctx), false));

  // вставка в начало
  BasicBlock &entry = F.getEntryBlock();
  IRBuilder<> builder(&*entry.getFirstInsertionPt());
  builder.CreateCall(startFunc);

  // вставка перед return
  for (auto &BB : F) {
    if (auto *ret = dyn_cast<ReturnInst>(BB.getTerminator())) {
      IRBuilder<> builder(ret);
      builder.CreateCall(endFunc);
    }
  }

  return PreservedAnalyses::none();
}
