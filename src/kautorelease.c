#include "kautorelease.h"

#include "karray.h"

K_THREAD_LOCAL KArrayRef tPool;

bool KAutoreleasePoolAddObject(KObjectRef obj) {
  if (!obj) {
    return false;
  }
  if (!tPool) {
    tPool = KArrayAlloc();
  }
  return KArrayAddObject(tPool, obj);
}

size_t KAutoreleasePoolPop() {
  if (!tPool) {
    return 0u;
  }
  return KArrayRemoveAllObjects(tPool);
}

KObjectRef KAutoreleaseMove(KObjectRef obj) {
  bool added = KAutoreleasePoolAddObject(obj);
  KObjectRelease(obj);
  if (!added) {
    return NULL;
  }
  return obj;
}
