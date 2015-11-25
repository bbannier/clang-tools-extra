//===--- ProcessOnStackCheck.cpp - clang-tidy------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ProcessOnStackCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

void ProcessOnStackCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(namedDecl(hasName("spawn"))),
               hasArgument(0, unaryOperator(hasOperatorName("&"))))
          .bind("spawn"),
      this);
}

void ProcessOnStackCheck::check(const MatchFinder::MatchResult &Result) {
  auto spawn = Result.Nodes.getNodeAs<CallExpr>("spawn");
  diag(spawn->getLocStart().getLocWithOffset(6),
       "do not spawn processes on the stack");
}

} // namespace tidy
} // namespace clang

