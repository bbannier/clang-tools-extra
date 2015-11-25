// RUN: %check_clang_tidy %s mesos-process-on-stack %t

namespace process {
struct UPID {};
struct ProcessBase {};
UPID spawn(ProcessBase*, bool = false) { return UPID(); }
} // namespace process

using process::ProcessBase;
using process::spawn;

void a() {
  ProcessBase process;
  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: do not spawn processes on the stack [mesos-process-on-stack]
  spawn(&process);
}

// FIXME(bbannier) This might need to go into a StaticAnalysis checker.
// void b() {
//   ProcessBase process;
//   ProcessBase* p = process;
//   // CHECK-MESSAGES__: :[[@LINE+1]]:9: warning: do not spawn processes on the stack [mesos-process-on-stack]
//   spawn(p);
// }

void good() {
  ProcessBase *process = new ProcessBase;
  spawn(process);
  delete process;
}
