// RUN: %check_clang_tidy %s mesos-this-capture %t

namespace lambda {
template <typename R, typename... Args>
struct function {
  function() = default;
  template <typename F>
  function(F f) {}
  // `function` needs to be non-POD so a `CXXBindTemporaryExpr` is emitted.
  ~function() {}
};
} // namespace lambda  {

struct Nothing {};

namespace process {
template <typename T>
struct Future {
  template <typename AnyCallback>
  Future onAny(AnyCallback &&) { return {}; };

  Future then(const lambda::function<void(const Future<T> &)> &) {
    return {}; };
};

template <typename F>
Future<Nothing> defer(int pid, F) { return {}; }

template <typename PID, typename Iterate, typename Body>
Future<Nothing> loop(const PID& pid, Iterate&& iterate, Body&& body);
} // namespace process  {

using process::Future;
using process::defer;
using process::loop;

struct S {
  Future<Nothing> future() const { return {}; }

  // TODO(bbannier): Rework matcher to check all branches of a chained Future.

  void f() {
    future()
        // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        .then([this]() { (void)this; });
    future()
        // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        .onAny([this]() { (void)this; });

    lambda::function<void()> f;
    future()
        // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        .onAny([this, f]() { (void)this; });
    }

    void g() {
      future()
          // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
          .then([=]() { (void)this; });
      future()
          // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
          .onAny([=]() { (void)this->i; });

      lambda::function<void()> f;
      future()
          // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
          .onAny([=]() { (void)this->i; (void)f; });
    }

    void h() {
      {
        auto l = [=]() { (void)this; };
        // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        future().then(l);
        // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        future().onAny(l);
      }

      {
        lambda::function<void()> f;
        auto l = [=]() { (void)this; (void)f; };

        // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
        future().onAny(l);
      }

      // // TODO(bbannier): Track chains of referenced lambdas over an arbitrary
      // // number of hops.
      // auto ll = l;
      // future.then(ll);
      // future.after(ll);
    }

    int i = 0;
};

struct P {
  Future<Nothing> future() const { return {}; }

  void f() {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
    loop(this, [this]() { (void)this; }, []() {});
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
    loop(this, []() {}, [this]() { (void)this; });

    auto l = [this]() { (void)this; };
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
    loop(this, l, []() {});
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: callback capturing this should be dispatched/deferred to a specific PID [mesos-this-capture]
    loop(this, []() {}, l);
  }
};

// Negatives.
void f() {
  Future<Nothing>().onAny([]() {});
  Future<Nothing>().then([]() {});
};

struct K {
    void f() {
      auto future = Future<Nothing>();

      future
          .then(defer(0, [=]() { (void)this; }));

      auto l = []() {};
      future.then(l);
    }
};
