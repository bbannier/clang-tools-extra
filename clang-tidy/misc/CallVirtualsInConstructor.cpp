#include "CallVirtualsInConstructor.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

CallVirtualsInConstructor::CallVirtualsInConstructor(StringRef Name,
                                                     ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void
CallVirtualsInConstructor::registerMatchers(ast_matchers::MatchFinder *Finder) {
  // match all constructors doing member function calls on this
  Finder->addMatcher(
      constructorDecl(hasDescendant(memberCallExpr(hasDescendant(thisExpr()))
                                        .bind("member_call"))).bind("ctr"),
      this);
  Finder->addMatcher(
      destructorDecl(hasDescendant(memberCallExpr(hasDescendant(thisExpr()))
                                       .bind("member_call"))).bind("dtr"),
      this);
}

void CallVirtualsInConstructor::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto member_call =
      Result.Nodes.getNodeAs<CXXMemberCallExpr>("member_call");

  // filter member calls to virtual functions
  if (not member_call->getMethodDecl()->isVirtual())
    return;

  if (Result.Nodes.getNodeAs<CXXConstructorDecl>("ctr"))
    diag(member_call->getLocStart(),
         "calling virtual member function in constructor");
  else if (Result.Nodes.getNodeAs<CXXDestructorDecl>("dtr"))
    diag(member_call->getLocStart(),
         "calling virtual member function in destructor");
}
} /* tidy */
} /* clang */
