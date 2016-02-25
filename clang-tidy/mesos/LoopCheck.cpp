//===--- LoopCheck.cpp - clang-tidy----------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LoopCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void LoopCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxForRangeStmt().bind("for"), this);
}

void LoopCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *for_ = Result.Nodes.getNodeAs<CXXForRangeStmt>("for");

  if (not for_)
    return;

  diag(for_->getLocStart(),
       "range-based for loops a C++ feature not allowed in mesos");
}

} // namespace mesos
} // namespace tidy
} // namespace clang
