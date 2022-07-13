#include "kmutex.h"

#include "kobject.h"
#include "ksemaphore.h"

K_IMPL_OBJECT(KMutex);

struct KMutex {
  KSemaphoreRef sema;
};

static void KMutexInit(KMutexRef mutex) {
  mutex->sema = KSemaphoreAlloc(1u);
}

static void KMutexDeInit(KMutexRef mutex) {
  KSemaphoreRelease(mutex->sema);
}

static KClass kMutexClass = {
    .init = (KClassInit)&KMutexInit,
    .deinit = (KClassDeinit)&KMutexDeInit,
    .size = sizeof(struct KMutex),
};

KMutexRef KMutexAlloc() {
  KMutexRef mutex = KObjectAlloc(&kMutexClass);
  if (!mutex) {
    return NULL;
  }
  if (!mutex->sema) {
    KMutexRelease(mutex);
    return NULL;
  }
  return mutex;
}

bool KMutexLock(KMutexRef mutex) {
  if (!mutex) {
    return false;
  }
  return KSemaphoreWait(mutex->sema);
}

bool KMutexUnlock(KMutexRef mutex) {
  if (!mutex) {
    return false;
  }
  return KSemaphoreSignal(mutex->sema);
}
