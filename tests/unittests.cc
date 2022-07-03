#include "gtest/gtest.h"
#include "k.h"

#include <string>

TEST(KObjectTest, Main) {
  typedef struct MyStruct {
    size_t stuff;
  } MyStruct;

  static bool hello;
  static uint32_t goodbye;

  hello = false;
  goodbye = 0;

  KClass my_class;
  my_class.init = [](void*) { hello = true; };
  my_class.deinit = [](void* obj) { goodbye = ((MyStruct*)obj)->stuff; };
  my_class.size = sizeof(MyStruct);

  ASSERT_FALSE(hello);
  ASSERT_EQ(goodbye, 0);
  KObjectRef obj = KObjectAlloc(&my_class);
  ASSERT_EQ(KObjectGetRetainCount(obj), 1u);
  ASSERT_EQ(((MyStruct*)obj)->stuff, 0u);  // Zero initialization.
  ((MyStruct*)obj)->stuff = 42;
  ASSERT_TRUE(hello);
  ASSERT_EQ(goodbye, 0);
  KObjectRetain(obj);
  ASSERT_EQ(KObjectGetRetainCount(obj), 2u);
  ASSERT_TRUE(hello);
  ASSERT_EQ(goodbye, 0);
  KObjectRelease(obj);
  ASSERT_EQ(KObjectGetRetainCount(obj), 1u);
  ASSERT_TRUE(hello);
  ASSERT_EQ(goodbye, 0);
  KObjectRelease(obj);
  ASSERT_TRUE(hello);
  ASSERT_EQ(goodbye, 42);
}

TEST(KObjectTest, KString) {
  KStringRef str = KStringAllocWithFormat("Hello %s", "World!");
  ASSERT_NE(str, nullptr);
  ASSERT_EQ(KObjectGetRetainCount(str), 1u);
  ASSERT_EQ(KStringGetLength(str), 12u);
  ASSERT_EQ(std::string(KStringGetData(str), KStringGetLength(str)),
            std::string("Hello World!"));
  KStringRelease(str);
}

TEST(KObjectTest, KAllocation) {
  KAllocationRef alloc = KAllocationAlloc();
  ASSERT_NE(alloc, nullptr);
  ASSERT_EQ(KObjectGetRetainCount(alloc), 1u);
  ASSERT_EQ(KAllocationGetSize(alloc), 0u);
  ASSERT_TRUE(KAllocationTruncate(alloc, 4096));
  ASSERT_EQ(KAllocationGetSize(alloc), 4096u);
  ASSERT_TRUE(KAllocationTruncate(alloc, 14));
  ASSERT_EQ(KAllocationGetSize(alloc), 14u);
  ASSERT_TRUE(KAllocationTruncate(alloc, 10000));
  ASSERT_EQ(KAllocationGetSize(alloc), 10000u);
  ASSERT_TRUE(KAllocationTruncate(alloc, 4096));
  ASSERT_EQ(KAllocationGetSize(alloc), 4096u);
  KAllocationRelease(alloc);
}

TEST(KObjectTest, KAllocationNPOT) {
  ASSERT_EQ(KAllocationNextPowerOfTwoSize(23), 32);
  ASSERT_EQ(KAllocationNextPowerOfTwoSize(64), 64);
  ASSERT_EQ(KAllocationNextPowerOfTwoSize(65), 128);
}

TEST(KObjectTest, KArray) {
  typedef struct MyStruct {
  } MyStruct;

  static uint32_t alloc_count;
  static uint32_t dealloc_count;

  alloc_count = 0;
  dealloc_count = 0;

  KClass my_class;
  my_class.init = [](void*) { alloc_count++; };
  my_class.deinit = [](void* obj) { dealloc_count++; };
  my_class.size = sizeof(MyStruct);

  KArrayRef array = KArrayAlloc();
  ASSERT_EQ(alloc_count, 0u);
  ASSERT_EQ(dealloc_count, 0u);
  KObjectRef object = KObjectAlloc(&my_class);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 0u);
  ASSERT_EQ(KArrayGetLength(array), 0u);
  KArrayAddObject(array, object);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 0u);
  ASSERT_EQ(KArrayGetLength(array), 1u);
  KObjectRelease(object);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 0u);
  KArrayRelease(array);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 1u);
  array = KArrayAlloc();
  KObjectRef object2 = KObjectAlloc(&my_class);
  ASSERT_EQ(alloc_count, 2u);
  ASSERT_EQ(dealloc_count, 1u);
  ASSERT_TRUE(KArrayAddObject(array, object2));
  KObjectRelease(object2);
  ASSERT_EQ(alloc_count, 2u);
  ASSERT_EQ(dealloc_count, 1u);
  ASSERT_TRUE(KArrayRemoveObjectAtIndex(array, 0));
  ASSERT_EQ(alloc_count, 2u);
  ASSERT_EQ(dealloc_count, 2u);
  KArrayRelease(array);
}

TEST(KObjectTest, KARP) {
  typedef struct MyStruct {
  } MyStruct;

  static uint32_t alloc_count;
  static uint32_t dealloc_count;

  alloc_count = 0;
  dealloc_count = 0;

  KClass my_class;
  my_class.init = [](void*) { alloc_count++; };
  my_class.deinit = [](void* obj) { dealloc_count++; };
  my_class.size = sizeof(MyStruct);

  KObjectRef object = KObjectAlloc(&my_class);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 0u);
  ASSERT_EQ(KAutoreleasePoolPop(), 0u);
  KAutoreleasePoolAddObject(object);
  KObjectRelease(object);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 0u);
  ASSERT_EQ(KAutoreleasePoolPop(), 1u);
  ASSERT_EQ(alloc_count, 1u);
  ASSERT_EQ(dealloc_count, 1u);
}
