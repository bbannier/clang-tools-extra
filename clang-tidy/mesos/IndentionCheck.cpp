//===--- IndentionCheck.cpp - clang-tidy-----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "IndentionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <type_traits>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void IndentionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasInitializer(expr().bind("rhs"))).bind("lhs"),
                     this);
  Finder->addMatcher(varDecl(has(cxxConstructExpr(hasDescendant(
                                 materializeTemporaryExpr().bind("rhs")))))
                         .bind("lhs"),
                     this);
  Finder->addMatcher(binaryOperator(hasOperatorName("="),
                                    hasLHS(expr().bind("lhs")),
                                    hasRHS(expr().bind("rhs"))),
                     this);
  Finder->addMatcher(cxxOperatorCallExpr(hasOverloadedOperatorName("="),
                                         hasArgument(0, expr().bind("lhs")),
                                         hasArgument(1, expr().bind("rhs"))),
                     this);
}

namespace {
struct Location {
  Location(unsigned int line, unsigned int column, FileID file)
      : line(line), column(column), file(file) {}
  const unsigned int line;
  const unsigned int column;
  const FileID file;
};

Location location(const SourceManager *SM, const SourceLocation &loc) {
  auto p = SM->getDecomposedLoc(loc);
  return {SM->getLineNumber(p.first, p.second),
          SM->getColumnNumber(p.first, p.second), p.first};
}
} // namespace

template <typename LHS_>
bool IndentionCheck::check_(const MatchFinder::MatchResult &Result) {
  bool matched = false;

  const auto *lhs = Result.Nodes.getNodeAs<LHS_>("lhs");
  const auto *rhs = Result.Nodes.getNodeAs<Expr>("rhs");
  if (not(lhs and rhs))
    return matched;

  matched = true;

  auto *SM = Result.SourceManager;
  const auto lhs_loc = location(SM, lhs->getLocStart());
  const auto rhs_loc = location(SM, rhs->getLocStart());

  // assignments are in different files (e.g., from macro expansions)
  if (lhs_loc.file != rhs_loc.file)
    return matched;

  // do not attempt to handle macro expansions
  if (lhs->getLocStart().isMacroID() or rhs->getLocStart().isMacroID())
    return matched;

  // assignment is on the same line
  if (lhs_loc.line == rhs_loc.line)
    return matched;

  // assignment in next line and correct column
  if (rhs_loc.column == lhs_loc.column + 2)
    return matched;

  diag(rhs->getLocStart(),
       "assignments should be indented by exactly two spaces");

  return matched;
}

void IndentionCheck::check(const MatchFinder::MatchResult &Result) {
  check_<Decl>(Result) or check_<Expr>(Result);
}

} // namespace mesos
} // namespace tidy
} // namespace clang
