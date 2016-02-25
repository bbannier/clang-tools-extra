// RUN: %check_clang_tidy %s mesos-const-variable %t

void f() {
  int a = 0; // OK
  const int b = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: const-qualified function-level local variables are strongly discouraged in mesos [mesos-const-variable]
}

const int g() {
  return 1;
}

auto c = g();

struct A {
};

const A &h() {
  static A a;
  return a;
}

#define FFF(a) const decltype(a) a_FFF = a;
FFF(10); // OK
