//===--- MesosTidyModule.cpp - clang-tidy ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "AnonymousNamespaceCheck.h"
#include "AutoCheck.h"
#include "ConstVariableCheck.h"
#include "LoopCheck.h"
#include "SmartPointersCheck.h"

#include "ExplicitFutureCheck.h"
#include "IndentionCheck.h"
#include "SwitchDefaultCheck.h"

namespace clang {
namespace tidy {
namespace mesos {

class MesosModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<AnonymousNamespaceCheck>(
        "mesos-anonymous-namespace");
    CheckFactories.registerCheck<AutoCheck>("mesos-auto");
    CheckFactories.registerCheck<ConstVariableCheck>("mesos-const-variable");
    CheckFactories.registerCheck<ExplicitFutureCheck>("mesos-explicit-future");
    CheckFactories.registerCheck<IndentionCheck>("mesos-indention");
    CheckFactories.registerCheck<LoopCheck>("mesos-loop");
    CheckFactories.registerCheck<SmartPointersCheck>("mesos-smart-pointers");
    CheckFactories.registerCheck<SwitchDefaultCheck>("mesos-switch-default");
  }
};

// Register the MesosModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<MesosModule>
    X("mesos-module", "Adds mesos-related checks.");

} // namespace mesos

// This anchor is used to force the linker to link in the generated object file
// and thus register the MesosModule.
volatile int MesosModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
