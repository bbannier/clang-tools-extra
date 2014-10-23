// RUN: clang-tidy -checks='-*,misc-virtual-shadowing' %s -- | FileCheck %s

namespace A1 {
struct A {
  void f() {}
  void g();
};

struct B : public A {
  // CHECK: :[[@LINE+1]]:3: warning: method hides non-virtual function from a base class [misc-virtual-shadowing]
  virtual void f() {} // match
};

// CHECK-NOT: warning
struct C : public B {
  virtual void f() {} // nomatch
};

// CHECK-NOT: warning
struct D : public B {
  virtual void f() {} // nomatch
};

// CHECK-NOT: warning
struct E {
  virtual void f() {} // nomatch
};
} // namespace A1

namespace A2 {
// CHECK-NOT: warning
struct A {};
struct B : public A {
  virtual void f() {} // nomatch
};
} // namespace A2
