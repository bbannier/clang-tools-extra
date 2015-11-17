set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyMesosModule
  ExplicitFutureCheck.cpp
  MesosTidyModule.cpp
  SwitchDefaultCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTooling
  )
