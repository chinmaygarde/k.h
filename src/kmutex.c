#include "kmutex.h"

#include "kobject.h"
#include "ksemaphore.h"

struct KMutex {
  KSemaphoreRef sema;
};

static void KMutexInit(KMutexRef mutex) {
  mutex->sema = KSemaphoreNew(1u);
}

static void KMutexDeInit(KMutexRef mutex) {
  KSemaphoreRelease(mutex->sema);
}

K_IMPL_OBJECT(KMutex);

KMutexRef KMutexNew() {
  KMutexRef mutex = KMutexAlloc();
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
