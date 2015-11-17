mesos-explicit-future
=====================

This checks that futures are never explicitly created in user code. Instead
futures should be created implicitly. This isolates business logic in the
function body from the decision on whether to expose that asynchronicity or
not.
