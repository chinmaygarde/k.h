#include "karray.h"

#include "kallocation.h"

struct KArray {
  size_t length;
  KAllocationRef items;
};

static void KArrayInit(KArrayRef array) {
  array->items = KAllocationNew();
}

static void KArrayDeInit(KArrayRef array) {
  KArrayRemoveAllObjects(array);
  KAllocationRelease(array->items);
}

K_IMPL_OBJECT(KArray);

KArrayRef KArrayNew() {
  KArrayRef array = KArrayAlloc();
  if (!array || !array->items) {
    KObjectRelease(array);
    return NULL;
  }
  return array;
}

bool KArrayAddObject(KArrayRef array, KObjectRef object) {
  if (!array || !object) {
    return false;
  }
  if (!KAllocationTruncate(array->items,
                           KAllocationNextPowerOfTwoSize((array->length + 1) *
                                                         sizeof(KObjectRef)))) {
    return false;
  };
  KObjectRetain(object);
  ((KObjectRef*)KAllocationGetBuffer(array->items))[array->length] = object;
  array->length += 1;
  return true;
}

bool KArrayRemoveObjectAtIndex(KArrayRef array, size_t index) {
  if (array == NULL || index >= array->length) {
    return false;
  }
  KObjectRef* buffer = KAllocationGetBuffer(array->items);
  KObjectRelease(buffer[index]);
  for (size_t i = index + 1; i < array->length; i++) {
    buffer[i - 1] = buffer[i];
  }
  array->length--;
  return true;
}

KObjectRef KArrayGetObjectAtIndex(KArrayRef array, size_t index) {
  if (array == NULL || index >= array->length) {
    return NULL;
  }
  return ((KObjectRef*)KAllocationGetBuffer(array->items))[index];
}

size_t KArrayGetLength(KArrayRef array) {
  if (!array) {
    return 0;
  }
  return array->length;
}

size_t KArrayRemoveAllObjects(KArrayRef array) {
  KObjectRef* items = KAllocationGetBuffer(array->items);
  for (size_t i = 0; i < array->length; i++) {
    KObjectRelease(items[i]);
  }
  size_t removed = array->length;
  array->length = 0u;
  return removed;
}

size_t KArrayIterate(KArrayRef array,
                     KArrayIterator iterator,
                     void* user_data) {
  if (!array || !iterator) {
    return 0u;
  }
  size_t iterated = 0;
  for (size_t i = 0, count = KArrayGetLength(array); i < count; i++) {
    iterated++;
    if (!iterator(KArrayGetObjectAtIndex(array, i), i, user_data)) {
      break;
    }
  }
  return iterated;
}
