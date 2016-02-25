//===--- ConstVariableCheck.cpp - clang-tidy-------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ConstVariableCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void ConstVariableCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("decl"), this);
}

void ConstVariableCheck::check(const MatchFinder::MatchResult &Result) {
  const auto* var = Result.Nodes.getNodeAs<VarDecl>("decl");

  if (not var)
    return;

  // don't warn about const in macro expansions
  if (var->getLocStart().isMacroID())
    return;

  if (not var->getType().getCanonicalType().isConstQualified())
    return;

  diag(var->getLocation(), "const-qualified function-level local variables are "
                           "strongly discouraged in mesos");
}

} // namespace mesos
} // namespace tidy
} // namespace clang
