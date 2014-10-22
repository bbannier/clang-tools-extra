#ifndef VIRTUALSHADOWING_H_OZFHBZGX
#define VIRTUALSHADOWING_H_OZFHBZGX

#include "../ClangTidy.h"

namespace clang {
namespace tidy {

class VirtualShadowing : public ClangTidyCheck {
public:
  VirtualShadowing(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // tidy
} // clang

#endif /* end of include guard: VIRTUALSHADOWING_H_OZFHBZGX */
