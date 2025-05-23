//===--- ClangTidyProfiling.cpp - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ClangTidyProfiling.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>
#include <system_error>
#include <utility>

#define DEBUG_TYPE "clang-tidy-profiling"

namespace clang::tidy {

ClangTidyProfiling::StorageParams::StorageParams(llvm::StringRef ProfilePrefix,
                                                 llvm::StringRef SourceFile)
    : Timestamp(std::chrono::system_clock::now()), SourceFilename(SourceFile) {
  llvm::SmallString<32> TimestampStr;
  llvm::raw_svector_ostream OS(TimestampStr);
  llvm::format_provider<decltype(Timestamp)>::format(Timestamp, OS,
                                                     "%Y%m%d%H%M%S%N");

  llvm::SmallString<256> FinalPrefix(ProfilePrefix);
  llvm::sys::path::append(FinalPrefix, TimestampStr);

  // So the full output name is: /ProfilePrefix/timestamp-inputfilename.json
  StoreFilename = llvm::Twine(FinalPrefix + "-" +
                              llvm::sys::path::filename(SourceFile) + ".json")
                      .str();
}

void ClangTidyProfiling::printUserFriendlyTable(llvm::raw_ostream &OS,
                                                llvm::TimerGroup &TG) {
  TG.print(OS);
  OS.flush();
}

void ClangTidyProfiling::printAsJSON(llvm::raw_ostream &OS,
                                     llvm::TimerGroup &TG) {
  OS << "{\n";
  OS << R"("file": ")" << Storage->SourceFilename << "\",\n";
  OS << R"("timestamp": ")" << Storage->Timestamp << "\",\n";
  OS << "\"profile\": {\n";
  TG.printJSONValues(OS, "");
  OS << "\n}\n";
  OS << "}\n";
  OS.flush();
}

void ClangTidyProfiling::storeProfileData(llvm::TimerGroup &TG) {
  assert(Storage && "We should have a filename.");

  llvm::SmallString<256> OutputDirectory(Storage->StoreFilename);
  llvm::sys::path::remove_filename(OutputDirectory);
  if (std::error_code EC = llvm::sys::fs::create_directories(OutputDirectory)) {
    llvm::errs() << "Unable to create output directory '" << OutputDirectory
                 << "': " << EC.message() << "\n";
    return;
  }

  std::error_code EC;
  llvm::raw_fd_ostream OS(Storage->StoreFilename, EC, llvm::sys::fs::OF_None);
  if (EC) {
    llvm::errs() << "Error opening output file '" << Storage->StoreFilename
                 << "': " << EC.message() << "\n";
    return;
  }

  printAsJSON(OS, TG);
}

ClangTidyProfiling::ClangTidyProfiling(std::optional<StorageParams> Storage)
    : Storage(std::move(Storage)) {}

ClangTidyProfiling::~ClangTidyProfiling() {
  llvm::TimerGroup TG{"clang-tidy", "clang-tidy checks profiling", Records};
  if (!Storage)
    printUserFriendlyTable(llvm::errs(), TG);
  else
    storeProfileData(TG);
}

} // namespace clang::tidy
