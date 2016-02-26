// RUN: %check_clang_tidy %s mesos-auto %t

namespace ns1 {
int f() {
  auto i = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: do not use auto, lock in types instead [mesos-auto]
  return i;
}

auto i = f();
// XXX-MESSAGES: :[[@LINE-1]]:1: warning: do not use auto, lock in types instead [mesos-auto]

auto g() -> int { return 0; }
// XXX-MESSAGES: :[[@LINE-1]]:1: warning: do not use auto, lock in types instead [mesos-auto]
} // namespace

namespace ns2 {
template <typename T>
auto t(T t) -> decltype(T::value) { // OK, dependent type
  auto i = t.value; // OK, dependent type
  return i;
}

struct A {
  int value = 0;
};

auto i = t(A());
// XXX-MESSAGES: :[[@LINE-1]]:1: warning: do not use auto, lock in types instead [mesos-auto]
}  // namespace
