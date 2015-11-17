//===--- SwitchDefaultCheck.cpp - clang-tidy-------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SwitchDefaultCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

void SwitchDefaultCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(has(declRefExpr(hasType(enumDecl()))),
                 has(compoundStmt(has(defaultStmt().bind("default"))))),
      this);
}

void SwitchDefaultCheck::check(const MatchFinder::MatchResult &Result) {
  auto default_ = Result.Nodes.getNodeAs<DefaultStmt>("default");

  diag(default_->getLocStart(),
       "do not use default when switching over enum values");
}

} // namespace tidy
} // namespace clang

