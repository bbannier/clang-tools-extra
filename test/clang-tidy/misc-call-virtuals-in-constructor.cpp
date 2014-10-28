// RUN: clang-tidy -checks='-*,misc-call-virtuals-in-constructor' %s -- | FileCheck %s

struct A {
  void f() {}
  virtual void virt() {}

  A(int) {
    f();
  }

  A(short) {
    // CHECK: :[[@LINE+1]]:5: warning: calling virtual member function in constructor [misc-call-virtuals-in-constructor]
    virt();
  }

  // CHECK: :[[@LINE+1]]:10: warning: calling virtual member function in destructor [misc-call-virtuals-in-constructor]
  ~A() { virt(); }
};

struct B : public A {
  B(int i) : A(i) {}
  B(short i) : A(i) {}
  B(): A(0) {
    // CHECK: :[[@LINE+1]]:5: warning: calling virtual member function in constructor [misc-call-virtuals-in-constructor]
    virt();
  }
};

struct C {
  C() {
    A a = A(0);
    // CHECK-NOT: :[[@LINE+1]]:5: warning
    a.virt();
  }
};

struct D {
  A a;
  // CHECK-NOT: :[[@LINE+1]]:23: warning
  D(int i) : a(i) { a.virt(); }
};

int main() {
  D d = D(10);
}

