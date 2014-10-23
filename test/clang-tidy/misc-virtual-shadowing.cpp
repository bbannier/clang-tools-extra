// // RUN: clang-tidy -checks='-*,misc-virtual-shadowing' %s -- | FileCheck %s

namespace A1 {
struct A {
  void f() {}
  void g();
};

struct B : public A {
  // CHECK: :[[@LINE+1]]:3: warning: method hides non-virtual function from a base class [misc-virtual-shadowing]
  virtual void f() {} // match
};

struct C : public B {
  // CHECK-NOT: :[[@LINE+1]]:0: warning
  virtual void f() {} // nomatch
};

struct D : public B {
  // CHECK-NOT: :[[@LINE+1]]:0: warning
  virtual void f() {} // nomatch
};

struct E {
  // CHECK-NOT: :[[@LINE+1]]:0: warning
  virtual void f() {} // nomatch
};
} // namespace A1

namespace A2 {
struct A {};
struct B : public A {
  // CHECK-NOT: :[[@LINE+1]]:0: warning
  virtual void f() {} // nomatch
};
} // namespace A2

namespace A3 {
  struct A {
    void f() {}
  };
  template <typename T>
  struct B : public T {
    // CHECK: :[[@LINE+1]]:5: warning: method hides non-virtual function from a base class [misc-virtual-shadowing]
    virtual void f() {}
  };

  B<A> b;
}

namespace A4 {
struct A {
  void f() {}
};
struct B {};
struct C : public B {
  // CHECK: :[[@LINE+1]]:3: warning: method hides non-virtual function from a base class [misc-virtual-shadowing]
  virtual void f() {}
};
} // namespace A4

