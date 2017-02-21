//===--- ThisCaptureCheck.cpp - clang-tidy---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ThisCaptureCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "llvm/ADT/STLExtras.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

AST_MATCHER(LambdaExpr, capturesThis) {
  return llvm::any_of(Node.captures(),
                      [](const LambdaCapture &c) { return c.capturesThis(); });
}

void ThisCaptureCheck::registerMatchers(MatchFinder *Finder) {
  const auto futureCallbackName = anyOf(
      hasName("after"),
      hasName("onAny"),
      hasName("onDiscard"),
      hasName("onDiscarded"),
      hasName("onFailed"),
      hasName("onReady"),
      hasName("repair"),
      hasName("then"));

  // Matchers for a this-capturing lambda, or a variable bound to such a lambda.
  const auto lambda = has(lambdaExpr(capturesThis()).bind("lambda"));

  // Depending on how the callback is being passed and whether it captures
  // additional variables with non-POD, non-trivial types, an additional
  // `CXXBindTemporaryExpr` might be emitted.
  const auto lambdaCapturingThis =
      anyOf(lambda, has(cxxBindTemporaryExpr(lambda)));

  const auto lambdaCapturingThisRef =
      declRefExpr(
          hasDeclaration(varDecl(has(exprWithCleanups(has(cxxConstructExpr(
              has(materializeTemporaryExpr(lambdaCapturingThis)))))))))
          .bind("ref");

  // Matcher for process::Future callbacks.
  // We need two submatchers as `.then` callbacks are passed by const ref,
  // while all others are passed by rvalue.
  const auto callbackByConstRef =
      materializeTemporaryExpr(ignoringImpCasts(has(cxxBindTemporaryExpr(
          has(ignoringImpCasts(cxxConstructExpr(has(cxxConstructExpr(
              anyOf(has(materializeTemporaryExpr(lambdaCapturingThis)),
                    has(ignoringImpCasts(lambdaCapturingThisRef))))))))))));

  const auto callbackByRvalue =
      anyOf(lambdaCapturingThis, lambdaCapturingThisRef);

  Finder->addMatcher(
      cxxMemberCallExpr(hasDeclaration(namedDecl(futureCallbackName)),
                        on(hasType(cxxRecordDecl(hasName("Future")))),
                        hasAnyArgument(anyOf(callbackByConstRef,
                                             callbackByRvalue))),
      this);

  // Matcher for process::loop.
  Finder->addMatcher(callExpr(callee(namedDecl(hasName("process::loop"))),
                              hasAnyArgument(anyOf(
                                  materializeTemporaryExpr(lambdaCapturingThis),
                                  lambdaCapturingThisRef))),
                     this);
}

void ThisCaptureCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ref = Result.Nodes.getNodeAs<Expr>("ref");
  const auto *lambda = Result.Nodes.getNodeAs<Expr>("lambda");

  diag(ref ? ref->getLocStart() : lambda->getLocStart(),
       "callback capturing this should be "
       "dispatched/deferred to a specific PID");

  // If the lambda was not declared at the site of the use add a note
  // at its declaration.
  if (ref && ref->getExprLoc() != lambda->getExprLoc()) {
    diag(lambda->getExprLoc(), "declared here", DiagnosticIDs::Note);
  }
}

} // namespace mesos
} // namespace tidy
} // namespace clang
