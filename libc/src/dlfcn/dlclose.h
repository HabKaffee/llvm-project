//===-- Implementation header of dlclose ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_DLFCN_DLCLOSE_H
#define LLVM_LIBC_SRC_DLFCN_DLCLOSE_H

namespace LIBC_NAMESPACE {

int dlclose(void *);

} // namespace LIBC_NAMESPACE

#endif // LLVM_LIBC_SRC_DLFCN_DLCLOSE_H
