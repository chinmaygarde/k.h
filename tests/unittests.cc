#include "gtest/gtest.h"
#include "k.h"

#include <string>
#include <thread>

TEST(KObjectTest, Main) {
  typedef struct MyStruct {
    size_t stuff;
  } MyStruct;

  static bool hello;
  static size_t goodbye;

  hello = false;
  goodbye = 0;

  KClass my_class;
  my_class.init = [](void*) { hello = true; };
  my_class.deinit = [](void* obj) { goodbye = ((MyStruct*)obj)->stuff; };
  my_class.size = sizeof(MyStruct);

  ASSERT_FALSE(hello);
  ASSERT_EQ(goodbye, 0);
  KObjectRef obj = KObjectNew(&my_class);
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
  KStringRef str = KStringNewWithFormat("Hello %s", "World!");
  ASSERT_NE(str, nullptr);
  ASSERT_EQ(KObjectGetRetainCount(str), 1u);
  ASSERT_EQ(KStringGetLength(str), 12u);
  ASSERT_EQ(std::string(KStringGetData(str), KStringGetLength(str)),
            std::string("Hello World!"));
  KStringRelease(str);
}

TEST(KObjectTest, KAllocation) {
  KAllocationRef alloc = KAllocationNew();
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
  my_class.deinit = [](void*) { dealloc_count++; };
  my_class.size = sizeof(MyStruct);

  KArrayRef array = KArrayNew();
  ASSERT_EQ(alloc_count, 0u);
  ASSERT_EQ(dealloc_count, 0u);
  KObjectRef object = KObjectNew(&my_class);
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
  array = KArrayNew();
  KObjectRef object2 = KObjectNew(&my_class);
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

  KObjectRef object = KObjectNew(&my_class);
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

TEST(KObjectTest, KLogging) {
  KLog(KLogLevelInfo, "Hello");
  KLog(KLogLevelInfo, "%s", "Hello Info");
  KLog(KLogLevelWarning, "%s", "Hello Warning");
  KLog(KLogLevelError, "%s", "Hello Error");
  K_LOG_INFO("Hi!");
  K_LOG_INFO("%s", "Hi! Info");
  K_LOG_WARNING("%s", "Hi! Warn");
  K_LOG_ERROR("%s", "Hi! Err");
  K_DLOG_INFO("%s", "Hi! Info DEBUG");
  K_DLOG_WARNING("%s", "Hi! Warn DEBUG");
  K_DLOG_ERROR("%s", "Hi! Err DEBUG");
}

TEST(KObjectTest, KHash) {
  KStringRef a = KStringNewWithFormat("Hello");
  KStringRef b = KStringNewWithFormat("Hello");
  ASSERT_EQ(KStringGetHash(a), KStringGetHash(b));
  KStringRelease(a);
  KStringRelease(b);
}

TEST(KObjectTest, KEqual) {
  KStringRef a = KStringNewWithFormat("Hello");
  KStringRef b = KStringNewWithFormat("Hello");
  ASSERT_TRUE(KStringIsEqual(a, b));
  KStringRelease(a);
  KStringRelease(b);
}

TEST(KObjectTest, KThread) {
  struct Captures {
    KMutexRef mutex;
    const char* argument;
  } captures = {};
  captures.argument = "Hello";
  captures.mutex = KMutexNew();
  ASSERT_NE(captures.mutex, nullptr);
  ASSERT_TRUE(KMutexLock(captures.mutex));
  KThreadRef thread = KThreadNew(
      [](void* arg) {
        Captures* captures = (Captures*)(arg);
        K_LOG_INFO("Thread says: %s", captures->argument);
        ASSERT_TRUE(KMutexUnlock(captures->mutex));
      },
      &captures);
  ASSERT_NE(thread, nullptr);
  ASSERT_TRUE(KMutexLock(captures.mutex));
  KThreadRelease(thread);
  KMutexRelease(captures.mutex);
}

TEST(KObjectTest, KHardwareConcurrency) {
  K_LOG_INFO("Hardware concurrency: %zu", KThreadGetHardwareConcurrency());
  ASSERT_EQ(std::thread::hardware_concurrency(),
            KThreadGetHardwareConcurrency());
}

TEST(KObjectTest, KMap) {
  KMapRef map = KMapNew((KMapHash)&KStringGetHash, (KMapEqual)&KStringIsEqual);
  ASSERT_EQ(KMapGetCount(map), 0u);
  KStringRef value = KStringNewWithFormat("MyValue");
  for (size_t i = 0; i < 1000; i++) {
    KStringRef key = KStringNewWithFormat("Key%zu", i);
    ASSERT_TRUE(KMapSetValue(map, key, value));
    KStringRelease(key);
  }
  for (size_t i = 0; i < 1000; i++) {
    KStringRef key = KStringNewWithFormat("Key%zu", i);
    KStringRef found_value = (KStringRef)KMapGetValue(map, key);
    ASSERT_NE(found_value, nullptr);
    ASSERT_TRUE(KStringIsEqual(found_value, value));
    KStringRelease(key);
  }
  for (size_t i = 0; i < 1000; i++) {
    KStringRef key = KStringNewWithFormat("Key%zu", i);
    ASSERT_TRUE(KMapRemoveValue(map, key));
    KStringRelease(key);
  }
  KMapRelease(map);
  KStringRelease(value);
}

TEST(KObjectTest, KTimeAndSleep) {
  const double kSleepTime = 0.1;
  for (size_t i = 0; i < 3; i++) {
    double time = KTimeGetCurrentSeconds();
    KThreadSleepSeconds(kSleepTime);
    ASSERT_GE(KTimeGetCurrentSeconds() - time, kSleepTime);
  }
}

TEST(KObjectTest, KConditionVariableOne) {
  static const double kSleepTime = 0.25;
  KConditionVariableRef cv = KConditionVariableNew();
  ASSERT_NE(cv, nullptr);
  double time = KTimeGetCurrentSeconds();
  KThreadRef thread = KThreadNew(
      [](void* user_data) {
        KThreadSleepSeconds(kSleepTime);
        ASSERT_TRUE(
            KConditionVariableNotifyOne((KConditionVariableRef)user_data));
      },
      cv);
  ASSERT_TRUE(KConditionVariableCriticalSectionEnter(cv));
  ASSERT_TRUE(KConditionVariableWait(
      cv,
      [](void* user_data) -> bool {
        return KTimeGetCurrentSeconds() - *((double*)user_data) < kSleepTime;
      },
      &time));
  ASSERT_TRUE(KConditionVariableCriticalSectionExit(cv));
  ASSERT_GE(KTimeGetCurrentSeconds() - time, kSleepTime);
  KConditionVariableRelease(cv);
  KThreadRelease(thread);
}

TEST(KObjectTest, KConditionVariableAll) {
  static const double kSleepTime = 0.25;
  KConditionVariableRef cv = KConditionVariableNew();
  ASSERT_NE(cv, nullptr);
  double time = KTimeGetCurrentSeconds();
  KThreadRef thread = KThreadNew(
      [](void* user_data) {
        KThreadSleepSeconds(kSleepTime);
        ASSERT_TRUE(
            KConditionVariableNotifyAll((KConditionVariableRef)user_data));
      },
      cv);
  ASSERT_TRUE(KConditionVariableCriticalSectionEnter(cv));
  ASSERT_TRUE(KConditionVariableWait(
      cv,
      [](void* user_data) -> bool {
        return KTimeGetCurrentSeconds() - *((double*)user_data) < kSleepTime;
      },
      &time));
  ASSERT_TRUE(KConditionVariableCriticalSectionExit(cv));
  ASSERT_GE(KTimeGetCurrentSeconds() - time, kSleepTime);
  KConditionVariableRelease(cv);
  KThreadRelease(thread);
}

TEST(KObjectTest, KList) {
  KListRef list = KListNew();
  ASSERT_NE(list, nullptr);

  KStringRef a = KStringNewWithFormat("A");
  KStringRef b = KStringNewWithFormat("B");
  KStringRef c = KStringNewWithFormat("C");
  KStringRef d = KStringNewWithFormat("D");

  ASSERT_EQ(KListGetCount(list), 0u);
  ASSERT_TRUE(KListAddObject(list, a));
  ASSERT_EQ(KListGetCount(list), 1u);
  ASSERT_TRUE(KListAddObject(list, b));
  ASSERT_EQ(KListGetCount(list), 2u);
  ASSERT_TRUE(KListAddObject(list, c));
  ASSERT_EQ(KListGetCount(list), 3u);
  ASSERT_TRUE(KListAddObject(list, d));
  ASSERT_EQ(KListGetCount(list), 4u);

  ASSERT_EQ(KListRemoveAllObjects(list), 4u);

  ASSERT_EQ(KListGetCount(list), 0u);
  ASSERT_TRUE(KListAddObject(list, a));
  ASSERT_EQ(KListGetCount(list), 1u);
  ASSERT_TRUE(KListAddObject(list, b));
  ASSERT_EQ(KListGetCount(list), 2u);
  ASSERT_TRUE(KListAddObject(list, c));
  ASSERT_EQ(KListGetCount(list), 3u);
  ASSERT_TRUE(KListAddObject(list, d));
  ASSERT_EQ(KListGetCount(list), 4u);

  ASSERT_EQ(KListGetObjectAtIndex(list, 0), a);
  ASSERT_EQ(KListGetObjectAtIndex(list, 1), b);
  ASSERT_EQ(KListGetObjectAtIndex(list, 2), c);
  ASSERT_EQ(KListGetObjectAtIndex(list, 3), d);
  ASSERT_EQ(KListGetObjectAtIndex(list, 4), nullptr);

  {
    KStringRef removed = (KStringRef)KListRemoveObjectAtIndex(list, 1u);
    ASSERT_EQ(removed, b);
    KStringRelease(removed);
    ASSERT_EQ(KListGetObjectAtIndex(list, 0), a);
    ASSERT_EQ(KListGetObjectAtIndex(list, 1), c);
    ASSERT_EQ(KListGetObjectAtIndex(list, 2), d);
    ASSERT_EQ(KListGetObjectAtIndex(list, 3), nullptr);
    ASSERT_EQ(KListGetCount(list), 3u);
  }

  {
    KStringRef removed = (KStringRef)KListRemoveObjectAtIndex(list, 0u);
    ASSERT_EQ(removed, a);
    KStringRelease(removed);
    ASSERT_EQ(KListGetObjectAtIndex(list, 0), c);
    ASSERT_EQ(KListGetObjectAtIndex(list, 1), d);
    ASSERT_EQ(KListGetObjectAtIndex(list, 2), nullptr);
    ASSERT_EQ(KListGetCount(list), 2u);
  }

  {
    KStringRef removed = (KStringRef)KListRemoveObjectAtIndex(list, 1u);
    ASSERT_EQ(removed, d);
    KStringRelease(removed);
    ASSERT_EQ(KListGetObjectAtIndex(list, 0), c);
    ASSERT_EQ(KListGetObjectAtIndex(list, 1), nullptr);
    ASSERT_EQ(KListGetCount(list), 1u);
  }

  KStringRelease(a);
  KStringRelease(b);
  KStringRelease(c);
  KStringRelease(d);

  KListRelease(list);
}

TEST(KObjectTest, KWorkerPool) {
  const size_t kTasksCount = 1000u;
  const size_t kWorkersCount = 8u;
  KCountdownLatchRef latch = KCountdownLatchNew(kTasksCount);
  KWorkerPoolRef pool = KWorkerPoolNew(kWorkersCount);
  ASSERT_NE(pool, nullptr);
  ASSERT_EQ(KWorkerPoolGetWorkerCount(pool), kWorkersCount);
  for (size_t i = 0; i < kTasksCount; i++) {
    ASSERT_TRUE(KWorkerPoolPostTask(
        pool,
        [](KObjectRef obj) {
          ASSERT_TRUE(KCountDownLatchCountDown(((KCountdownLatchRef)obj)));
        },
        latch));
  }
  KCountDownLatchWait(latch);
  KWorkerPoolRelease(pool);
  KCountdownLatchRelease(latch);
}

TEST(KObjectTest, KFilePath) {
  {
    KFilePathRef path = KFilePathNewWithFormat("a/b/c");
    ASSERT_EQ(KFilePathCountComponents(path), 3u);
    ASSERT_FALSE(KFilePathIsAbsolute(path));
    KFilePathRelease(path);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("hello");
    ASSERT_EQ(KFilePathCountComponents(path), 1u);
    ASSERT_FALSE(KFilePathIsAbsolute(path));
    KFilePathRelease(path);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("a/b/c/");
    ASSERT_EQ(KFilePathCountComponents(path), 3u);
    ASSERT_FALSE(KFilePathIsAbsolute(path));
    KStringRef str = KFilePathNewStringRepresentationWithSeparator(path, '/');
    KFilePathRelease(path);
    KStringRef comparison = KStringNewWithFormat("a/b/c");
    KStringIsEqual(str, comparison);
    KStringRelease(comparison);
    KStringRelease(str);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("a/////b/c/");
    ASSERT_EQ(KFilePathCountComponents(path), 3u);
    ASSERT_FALSE(KFilePathIsAbsolute(path));
    KStringRef str = KFilePathNewStringRepresentationWithSeparator(path, '/');
    KFilePathRelease(path);
    KStringRef comparison = KStringNewWithFormat("a/b/c");
    KStringIsEqual(str, comparison);
    KStringRelease(comparison);
    KStringRelease(str);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("a/x//y/");
    ASSERT_EQ(KFilePathCountComponents(path), 3u);
    ASSERT_FALSE(KFilePathIsAbsolute(path));
    KFilePathRelease(path);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("/a/b/c");
    ASSERT_EQ(KFilePathCountComponents(path), 3u);
    ASSERT_TRUE(KFilePathIsAbsolute(path));
    KFilePathRelease(path);
  }
  {
    KFilePathRef path = KFilePathNewWithFormat("/");
    ASSERT_EQ(KFilePathCountComponents(path), 0u);
    ASSERT_TRUE(KFilePathIsAbsolute(path));
    KFilePathRelease(path);
  }
}

TEST(KObjectTest, KFileHandle) {
  KFilePathRef path = KFilePathNewWithFormat("../fixtures/image.jpg");
  KFileHandleRef handle = KFileHandleNew(path, kFilePermissionReadOnly);
  ASSERT_NE(handle, nullptr);
  size_t size;
  ASSERT_TRUE(KFileHandleGetSize(handle, &size));
  ASSERT_EQ(size, 115292u);
  KMappingRef mapping = KFileHandleNewMapping(handle, size, kMapProtectionRead);
  ASSERT_NE(mapping, nullptr);
  ASSERT_EQ(KMappingGetSize(mapping), 115292u);
  ASSERT_NE(KMappingGetBuffer(mapping), nullptr);
  KMappingRelease(mapping);
  KFileHandleRelease(handle);
  KFilePathRelease(path);
}
