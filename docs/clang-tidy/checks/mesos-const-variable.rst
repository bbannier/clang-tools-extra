.. title:: clang-tidy - mesos-const-variable

mesos-const-variable
====================

Checks that function-local variables are not declared `const`. While not
strictly forbidden, informal Mesos style strongly discourages using
`const`-qualified function-local variables.
