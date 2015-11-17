mesos-switch-default
====================

This checks ensure that when switching over enum values no `default` branch is
present. The benefit of `switch` with enums is that compilers typically can
infer whether behavior for the whole domain of the type is specified, and will
consequentially warn if new values appear and a decision on their behavior is
needed. If a `default` branch is present always the whole domain will be
covered and the benefit from the implicit checking by the compiler is lost.
