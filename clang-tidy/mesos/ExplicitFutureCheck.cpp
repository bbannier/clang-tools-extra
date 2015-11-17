//===--- ExplicitFutureCheck.cpp - clang-tidy------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ExplicitFutureCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

void ExplicitFutureCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxConstructExpr(unless(anyOf(has(callExpr()),
                                                   hasAncestor(returnStmt()))))
                         .bind("ctr"),
                     this);
}

void ExplicitFutureCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ctr = Result.Nodes.getNodeAs<CXXConstructExpr>("ctr");

  if (ctr->getConstructor() and
      ctr->getConstructor()->getNameAsString() != "Future")
    // only care about construction of Futures
    return;

  if (ctr->getLocStart() == ctr->getLocEnd())
    // construction inserted implicitly
    return;

  diag(ctr->getLocation(), "Futures should only be created implicitly");
}

} // namespace tidy
} // namespace clang

