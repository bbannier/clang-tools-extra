// RUN: %check_clang_tidy %s mesos-switch-default %t

enum E { eA, eB, eC };

void f1(E e) {
  switch (e) {
  case eA:
  case eB:
  case eC:
    break;
    // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: do not use default when switching over enum values [mesos-switch-default]
  default:
    break;
  }
}

void f2(E e) {
  switch (e) {
  case eA:
  case eB:
  case eC:
    break;
  }
}
