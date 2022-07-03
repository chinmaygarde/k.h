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
