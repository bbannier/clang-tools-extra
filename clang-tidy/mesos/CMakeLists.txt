set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyMesosModule
  AnonymousNamespaceCheck.cpp
  ConstVariableCheck.cpp
  ExplicitFutureCheck.cpp
  IndentionCheck.cpp
  LoopCheck.cpp
  MesosTidyModule.cpp
  SmartPointersCheck.cpp
  SwitchDefaultCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTooling
  )
