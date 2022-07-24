# `k.h` [![KDotH](https://github.com/chinmaygarde/kdoth/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/chinmaygarde/kdoth/actions/workflows/cmake.yml)

A C11 library meant to be used a base library for other projects. Contains
utilities that I end up writing in one way or another over and over.

The umbrella header `k.h` should be all you need to import to use everything in
the library.

# Building

If you are using a `CMake` project, add the `kdoth` subdirectory and link the
`kdoth` library into your executable or library.

```
add_subdirectory(path_to_kdoth)
target_link_library(your_target kdoth)
```

In your C/C++ translation unit, include `k.h`

```
#include "k.h"
```

# Reference

## Object

All object are thread-safe reference-counted. Methods with `New` or `Copy` in
their names return a +1 out reference. This reference must be explicitly
released either via `KObjectRelease` or a typed wrapper the same (such as
`KStringRelease` or `KArrayRelease`).

References to objects can be added via `KObjectRetain`. Most object also define
typed wrappers to this method (such as `KStringRetain` or `KArrayRetain`).
