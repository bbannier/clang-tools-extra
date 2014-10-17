#ifndef CALLVIRTUALSINCONSTRUCTOR_H
#define CALLVIRTUALSINCONSTRUCTOR_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {

class CallVirtualsInConstructor : public ClangTidyCheck {
public:
  CallVirtualsInConstructor(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // tidy
} // clang

#endif /* end of include guard: CALLVIRTUALSINCONSTRUCTOR_H */
