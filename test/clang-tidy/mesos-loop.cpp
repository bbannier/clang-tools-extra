// RUN: %check_clang_tidy %s mesos-loop %t

#include <initializer_list>

void f() {
  for (auto i : {1, 2, 3, 4, 5}) {
    // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: range-based for loops a C++ feature not allowed in mesos [mesos-loop]
  }
}
