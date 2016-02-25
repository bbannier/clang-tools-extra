// RUN: %check_clang_tidy %s mesos-anonymous-namespace %t

namespace A {
int a = 0;
}  // namespace A  {

// CHECK-MESSAGES: :[[@LINE+1]]:11: warning: anonymous namespace are a C++ feature not allowed in mesos [mesos-anonymous-namespace]
namespace {
int i = 0;
} // namespace
