//===--- AnonymousNamespaceCheck.cpp - clang-tidy--------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AnonymousNamespaceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void AnonymousNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(namespaceDecl(isAnonymous()).bind("ns"), this);
}

void AnonymousNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  const auto* ns = Result.Nodes.getNodeAs<NamespaceDecl>("ns");

  if (not ns)
    return;

  diag(ns->getLocation(),
       "anonymous namespace are a C++ feature not allowed in mesos");
}

} // namespace mesos
} // namespace tidy
} // namespace clang
