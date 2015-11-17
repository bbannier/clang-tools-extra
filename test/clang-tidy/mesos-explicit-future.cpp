// RUN: python %S/check_clang_tidy.py %s mesos-explicit-future %t

namespace {
// the type we are matching explicit construction from
template <typename T>
struct Future {
  Future(T *x) {}
  Future(const T &x) {}
};

// just some type
struct S {
  S(int) {}
};
} // namespace

////////////// caught stuff below //////////////

// we should never explicitly construct Futures
void f() {
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: Futures should only be created implicitly [mesos-explicit-future]
  Future<int> f1 = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Futures should only be created implicitly [mesos-explicit-future]
  auto f2 = Future<int>(10);
}

////////////// OK stuff below //////////////

// implicit conversions in return are OK
Future<int> awesome_f2() {
  return 2;
}

// copy-constructing is OK
void awesome_f3() {
  auto f1 = awesome_f2();
  Future<int> f2 = awesome_f2();
}

// automatic conversions for call args are OK
int g(Future<int> f) {
  g(10);
}

// constructing other types is OK
int h() {
  S s(10);
}

