#include "VirtualShadowing.h"
#include <array>

using namespace clang::ast_matchers;

namespace clang {
namespace ast_matchers {
AST_MATCHER(CXXMethodDecl, isVirtualMember) {
  return Node.isVirtual();
}
} // namespace ast_matchers

namespace tidy {
VirtualShadowing::VirtualShadowing(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void VirtualShadowing::registerMatchers(ast_matchers::MatchFinder *Finder) {
  // match all virtual member functions which aren't overrides
  Finder->addMatcher(
      recordDecl(has(methodDecl(isVirtualMember()).bind("method"))), this);
}

namespace
{
bool CandidatePred(const CXXRecordDecl *BaseDefinition, void *UserData) {
  const auto method = reinterpret_cast<const CXXMethodDecl *>(UserData);
  const auto &name = method->getNameAsString();
  for (const auto &base_method : BaseDefinition->methods()) {
    // if we find a method with the same name check if it's virtual
    if (name == base_method->getNameAsString()) {
      return not base_method->isVirtual();
    }
  }

  // if this base has base classes check them also
  return BaseDefinition->getNumBases() != 0 and
         BaseDefinition->forallBases(CandidatePred, UserData);
}
} // namespace

void
VirtualShadowing::check(const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto method = Result.Nodes.getNodeAs<CXXMethodDecl>("method");
  const auto cl_decl = method->getParent();

  // filter out classes w/o base classes
  if (cl_decl->getNumBases() == 0)
    return;

  if (not cl_decl->forallBases(CandidatePred,
                               const_cast<CXXMethodDecl *>(method)))
    return;

  diag(method->getLocStart(),
       "method hides non-virtual function from a base class");
}
} /* tidy */
} /* clang */
