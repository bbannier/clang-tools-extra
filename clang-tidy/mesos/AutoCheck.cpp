//===--- AutoCheck.cpp - clang-tidy----------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AutoCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void AutoCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(autoType().bind("auto"), this);
}

void AutoCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *auto_ = Result.Nodes.getNodeAs<Decl>("auto");

  if (not auto_)
    return;

  diag(auto_->getLocation(),
       "auto declaration are a C++ feature not allowed in mesos");
}

} // namespace mesos
} // namespace tidy
} // namespace clang
