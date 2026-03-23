//===- InstrumentStartEnd.h - Function entry/exit instrumentation -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass instruments every function by inserting calls to
// instrument_start() at the function entry and instrument_end() before
// every return instruction.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_INSTRUMENTSTARTEND_H
#define LLVM_TRANSFORMS_UTILS_INSTRUMENTSTARTEND_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

class InstrumentStartEnd : public PassInfoMixin<InstrumentStartEnd> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_INSTRUMENTSTARTEND_H
