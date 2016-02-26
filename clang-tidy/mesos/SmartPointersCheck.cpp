//===--- SmartPointersCheck.cpp - clang-tidy-------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SmartPointersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void SmartPointersCheck::registerMatchers(MatchFinder *Finder) {
  const auto forbiddenSmartPtrType = namedDecl(
      anyOf(hasName("shared_ptr"), hasName("unique_ptr"), hasName("weak_ptr")));

  Finder->addMatcher(varDecl(hasType(forbiddenSmartPtrType)).bind("name"), this);
  Finder->addMatcher(
      functionDecl(returns(qualType(hasDeclaration(forbiddenSmartPtrType))))
          .bind("name"),
      this);
}

void SmartPointersCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *name = Result.Nodes.getNodeAs<NamedDecl>("name");

  if (not name)
    return;

  // FIXME(bbannier) emit diagnostic at actual type location, not at name.
  diag(name->getLocation(), "C++ smart pointers are discouraged in mesos");
}

} // namespace mesos
} // namespace tidy
} // namespace clang
