#include "ksemaphore.h"

#include "kobject.h"
#include "kplatform.h"

#if K_OS_WIN
#include <Windows.h>
#else  // K_OS_WIN
#include <semaphore.h>
#endif  // K_OS_WIN

struct KSemaphore {
#if K_OS_WIN
  HANDLE handle;
#else   // K_OS_WIN
  sem_t handle;
  bool valid;
#endif  // K_OS_WIN
};

void KSemaphoreInit(KSemaphoreRef sema) {}

void KSemaphoreDeInit(KSemaphoreRef sema) {
#if K_OS_WIN
  if (sema->handle) {
    CloseHandle(sema->handle);
  }
#else   // K_OS_WIN
  if (sema->valid) {
    sem_destroy(&sema->handle);
  }
#endif  // K_OS_WIN
}

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

#if K_OS_WIN
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
#else   // K_OS_WIN
  sem_t handle;
  if (sem_init(&handle, 0, count) != 0) {
    KSemaphoreRelease(sema);
    return NULL;
  }
  sema->handle = handle;
  sema->valid = true;
#endif  // K_OS_WIN

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
#if K_OS_WIN
  return WaitForSingleObject(sema->handle, INFINITE) == WAIT_OBJECT_0;
#else   // K_OS_WIN
  if (!sema->valid) {
    return false;
  }
  return K_HANDLE_EINTR(sem_wait(&sema->handle)) == 0;
#endif  // K_OS_WIN
}

bool KSemaphoreSignal(KSemaphoreRef sema) {
  if (!sema) {
    return false;
  }
#if K_OS_WIN
  return ReleaseSemaphore(sema->handle,  // HANDLE hSemaphore
                          1u,            // LONG   lReleaseCount
                          NULL           // LPLONG lpPreviousCount
                          ) == TRUE;
#else   // K_OS_WIN
  if (!sema->valid) {
    return false;
  }
  return sem_post(&sema->handle) == 0;
#endif  // K_OS_WIN
}
