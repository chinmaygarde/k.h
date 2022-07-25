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

## Objects

All objects are thread-safe reference-counted. Methods with `New`, `Alloc`, or
`Copy` in their names return a +1 out reference. This reference must be
explicitly released either via `KObjectRelease` or a typed wrapper the same
(such as `KStringRelease` or `KArrayRelease`).

References to objects can be added via `KObjectRetain`. Most object also define
typed wrappers to this method (such as `KStringRetain` or `KArrayRetain`).

### Defining Objects

To create your own objects, first define them in a header using the
`K_DEF_OBJECT` macro in a header:

```
K_DEF_OBJECT(Foo);
```

This implicitly defined the objects `Alloc` method along with typed `Retain` and
`Release` variants for the object. Effectively, the following new methods are
declared.

```
FooRef FooAlloc();
void FooRetain(FoorRef foo);
void FooRelease(FoorRef foo);
```

Then, in an implementation file, add an implementation for the object using
`K_IMPL_OBJECT` along with its `struct` and `Init` and `DeInit` methods.

```
struct Foo {
 // Whatever is part of the object.
};
K_IMPL_OBJECT(Foo);
void FooInit(FooRef foo) {
  // Called after each instance of foo is allocated. This is the object
  // "constructor".
}
void FooDeInit(FoorRef foo) {
  // Called before each instead of foo is deallocated. This is the object
  // "destructor".
}
```

## Platform detection.

Macros for detecting the platforms is are in `kplatform.h`. These include macros
like `K_OS_DARWIN`, `K_OS_WIN`, etc..

## Synchronization

* **KConditionVariableRef**: Condition variables.
* **KCountdownLatchRef**: Count down latches for waiting on the completion of a
  certain number of jobs.
* **KSemaphoreRef**: A counting semaphore.
* **KMutexRef**: A binary semaphore.

## Threading

* **KThreadRef**: An OS thread.
* Timing: Get the monotonic from the system high-resolution clock.
* **KWorkerPoolRef**: A pool of worker threads.

## Filesystem

* **KFileHandleRef**: A native file handle.
* **KFilePathRef**: A native file path.

## Logging

Utilities for logging such as `K_LOG_INFO`, `K_LOG_WARNING`, and `K_LOG_ERROR`.

## Utilities

* **KAllocationRef**: A contiguous allocation.
* Assertions: Macros like `K_ASSERT` and `K_DASSERT`.
* **KAutoreleaseRef**: A thread-local pool or auto-released objects. Not used
  internally by the library.
* **KHashRef**: Hashing utilities.
* **KMappingRef**: A pair of buffer pointer and buffer size. Used by other utilities
  to present a unified view of mappings.

## Collections

A collections hold a strong reference to the objects in them.

* **KArrayRef**: A contiguous array of objects.
* **KListRef**: A linked list of objects.
* **KMapRef**: A hash map.
* **KStringRef**: A C-string.
