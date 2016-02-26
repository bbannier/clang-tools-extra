// RUN: %check_clang_tidy %s mesos-smart-pointers %t

#include <memory>

std::shared_ptr<int> f();
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: C++ smart pointers are discouraged in mesos [mesos-smart-pointers]

std::unique_ptr<int> g();
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: C++ smart pointers are discouraged in mesos [mesos-smart-pointers]

using std::shared_ptr;
shared_ptr<int> sp;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: C++ smart pointers are discouraged in mesos [mesos-smart-pointers]

using std::unique_ptr;
unique_ptr<int> up;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: C++ smart pointers are discouraged in mesos [mesos-smart-pointers]

std::shared_ptr<int> sp_;
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: C++ smart pointers are discouraged in mesos [mesos-smart-pointers]
