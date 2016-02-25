// RUN: %check_clang_tidy %s mesos-indention %t

namespace ns1 {
int a =
  1; // OK
  int b =
    1; // OK
int c =
    1;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: assignments should be indented by exactly two spaces [mesos-indention]
  int d =
      1;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: assignments should be indented by exactly two spaces [mesos-indention]

void f(int a) {
  a =
    1; // OK
  a =
      1;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: assignments should be indented by exactly two spaces [mesos-indention]
  }
}  // namespace ns1

namespace ns2 {
struct A {
  /*implicit*/ A(int) {}
};

A a =
  1; // OK
A b =
    1;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: assignments should be indented by exactly two spaces [mesos-indention]
}  // namespace ns2

namespace ns3 {
struct A {
  /*implicit*/ A(int) {}
  A &operator=(int) { return *this; }
};
// A &operator=(A &a, int) { return a; }

void f(A a) {
  a =
    1; // OK
  a =
      1;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: assignments should be indented by exactly two spaces [mesos-indention]
  }
}  // namespace ns3

namespace ns3
{
#define NULL 0

void *p = NULL;

#undef NULL
} // namespace ns3
