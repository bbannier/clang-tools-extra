#include "CallVirtualsInConstructor.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

CallVirtualsInConstructor::CallVirtualsInConstructor(StringRef Name,
                                                     ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void
CallVirtualsInConstructor::registerMatchers(ast_matchers::MatchFinder *Finder) {
  // match all constructors doing member function calls
  Finder->addMatcher(
      constructorDecl(hasDescendant(memberCallExpr().bind("member_call"))),
      this);
}

void CallVirtualsInConstructor::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto member_call =
      Result.Nodes.getNodeAs<CXXMemberCallExpr>("member_call");

  // filter member calls to virtual functions
  if (not member_call->getMethodDecl()->isVirtual())
    return;

  diag(member_call->getLocStart(),
       "calling virtual member function in constructor");
}
} /* tidy */
} /* clang */
