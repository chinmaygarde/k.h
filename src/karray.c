#include "karray.h"
#include "kallocation.h"

#include <stdio.h>

struct KArray {
  size_t length;
  KAllocationRef items;
};

static void KArrayInit(KArrayRef array) {
  array->items = KAllocationAlloc();
}

static void KArrayDeinit(KArrayRef array) {
  KObjectRef* items = KAllocationGetBuffer(array->items);
  for (size_t i = 0; i < array->length; i++) {
    KObjectRelease(items[i]);
  }
  KAllocationRelease(array->items);
}

static KClass kArrayClass = {
    .init = (KClassInit)&KArrayInit,
    .deinit = (KClassDeinit)&KArrayDeinit,
    .size = sizeof(struct KArray),
};

KArrayRef KArrayAlloc() {
  KArrayRef array = KObjectAlloc(&kArrayClass);
  if (!array || !array->items) {
    KObjectRelease(array);
    return NULL;
  }
  return array;
}

bool KArrayAddObject(KArrayRef array, KObjectRef object) {
  if (!array || !object) {
    printf("Hello\n");
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
  return array->length;
}

void KArrayRetain(KArrayRef array) {
  KObjectRetain(array);
}

void KArrayRelease(KArrayRef array) {
  KObjectRelease(array);
}
