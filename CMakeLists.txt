cmake_minimum_required(VERSION 3.0)

project(kobject C CXX)

option(K_BUILD_TESTS "Build k.h unit-tests" ON)

set(C_STANDARD 11)
set(CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(kobject
  src/k.h
  src/kallocation.c
  src/kallocation.h
  src/karray.c
  src/karray.h
  src/kassert.c
  src/kassert.h
  src/kautorelease.c
  src/kautorelease.h
  src/khash.c
  src/khash.h
  src/klogging.c
  src/klogging.h
  src/kmacros.h
  src/kmutex.c
  src/kmutex.h
  src/kobject.c
  src/kobject.h
  src/kplatform.h
  src/ksemaphore.c
  src/ksemaphore.h
  src/kstring.c
  src/kstring.h
  src/kthread.c
  src/kthread.h
  src/ktypes.h
)

target_include_directories(kobject
  PUBLIC
    src
)

if(K_BUILD_TESTS)
  enable_testing()

  include(GoogleTest)

  add_subdirectory(third_party/googletest)

  add_executable(kobject_unittests
    tests/unittests.cc
    tests/cinclude.c
  )

  target_link_libraries(kobject_unittests
    PRIVATE
      gtest_main
      gtest
      kobject
  )

  gtest_discover_tests(kobject_unittests)
endif()

