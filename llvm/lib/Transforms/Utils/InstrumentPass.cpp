#include "llvm/Transforms/Utils/InstrumentPass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

using namespace llvm;

PreservedAnalyses InstrumentPass::run(Function &F, FunctionAnalysisManager &AM) {
  // 1. Пропускаем объявления функций (функции без тела)
  if (F.isDeclaration())
    return PreservedAnalyses::all();

  // 2. Игнорируем сами функции инструментации во избежание бесконечной рекурсии
  if (F.getName() == "instrument_start" || F.getName() == "instrument_end")
    return PreservedAnalyses::all();

  Module *M = F.getParent();
  LLVMContext &Ctx = M->getContext();
  
  // Создаем сигнатуру функции: void()
  Type *VoidTy = Type::getVoidTy(Ctx);
  FunctionType *FnTy = FunctionType::get(VoidTy, false);

  // Получаем или объявляем внешние функции в модуле
  FunctionCallee StartFn = M->getOrInsertFunction("instrument_start", FnTy);
  FunctionCallee EndFn = M->getOrInsertFunction("instrument_end", FnTy);

  bool Changed = false;

  // 3. Вставляем вызов instrument_start() в самое начало функции
  BasicBlock &EntryBB = F.getEntryBlock();
  IRBuilder<> Builder(&*EntryBB.getFirstInsertionPt());
  Builder.CreateCall(StartFn);
  Changed = true;

  // 4. Ищем все точки выхода из функции (инструкции return) и вставляем instrument_end()
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (ReturnInst *RI = dyn_cast<ReturnInst>(&I)) {
        Builder.SetInsertPoint(RI);
        Builder.CreateCall(EndFn);
      }
    }
  }

  // Если мы изменили IR, нужно сообщить, что старые анализы больше не валидны
  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}