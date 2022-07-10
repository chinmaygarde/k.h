#include "ksemaphore.h"

#include <Windows.h>
#include "kobject.h"

struct KSemaphore {
  HANDLE handle;
};

void KSemaphoreInit(KSemaphoreRef sema) {}

void KSemaphoreDeInit(KSemaphoreRef sema) {}

static KClass KSemaphoreClass = {
    .init = (KClassInit)&KSemaphoreInit,
    .deinit = (KClassDeinit)&KSemaphoreDeInit,
    .size = sizeof(struct KSemaphore),
};

KSemaphoreRef KSemaphoreAlloc(size_t count) {
  KSemaphoreRef sema = KObjectAlloc(&KSemaphoreClass);
  if (!sema) {
    return NULL;
  }

  HANDLE handle =
      CreateSemaphore(NULL,   // LPSECURITY_ATTRIBUTES lpSemaphoreAttributes
                      count,  // LONG lInitialCount
                      count,  // LONG lMaximumCount
                      NULL    // LPCSTR lpName
      );

  if (!handle) {
    KSemaphoreRelease(sema);
    return NULL;
  }

  sema->handle = handle;
  return sema;
}

void KSemaphoreRetain(KSemaphoreRef sema) {
  KObjectRetain(sema);
}

void KSemaphoreRelease(KSemaphoreRef sema) {
  KObjectRelease(sema);
}

bool KSemaphoreWait(KSemaphoreRef sema) {
  if (!sema) {
    return false;
  }
  return WaitForSingleObject(sema->handle, INFINITE) == WAIT_OBJECT_0;
}

bool KSemaphoreSignal(KSemaphoreRef sema) {
  if (!sema) {
    return false;
  }
  return ReleaseSemaphore(sema->handle,  // HANDLE hSemaphore
                          1u,            // LONG   lReleaseCount
                          NULL           // LPLONG lpPreviousCount
                          ) == TRUE;
}
