#include "kautorelease.h"

#include "karray.h"

_Thread_local KArrayRef tPool;

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

bool KAutoreleaseMove(KObjectRef obj) {
  bool added = KAutoreleasePoolAddObject(obj);
  if (!added) {
    return false;
  }
  KObjectRelease(obj);
  return true;
}
