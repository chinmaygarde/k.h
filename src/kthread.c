#include "kthread.h"

#include "klogging.h"
#include "kplatform.h"
#include "ktypes.h"

#if K_OS_WIN
#include <Windows.h>
#else  // K_OS_WIN
#include <pthread.h>
#include <unistd.h>
#endif  // K_OS_WIN

struct KThread {
  bool is_joined;
  KThreadProc main;
  void* user_data;
#if K_OS_WIN
  HANDLE handle;
#else   // K_OS_WIN
  pthread_t handle;
  bool is_valid;
#endif  // K_OS_WIN
};

#if K_OS_WIN

#else  // K_OS_WIN

#endif  // K_OS_WIN

static void KThreadInit(KThreadRef thread) {}

static void KThreadDeInit(KThreadRef thread) {
  KThreadJoin(thread);
}

static KClass KThreadClass = {
    .init = (KClassInit)&KThreadInit,
    .deinit = (KClassDeinit)&KThreadDeInit,
    .size = sizeof(struct KThread),
};

#if K_OS_WIN
static DWORD KThreadMain(LPVOID arg)
#else   // K_OS_WIN
static void* KThreadMain(void* arg)
#endif  // K_OS_WIN
{
  KThreadRef thread = (KThreadRef)arg;

  KThreadProc main = thread->main;
  void* user_data = thread->user_data;

  KThreadRelease(thread);

  main(user_data);

  return 0;
}

KThreadRef KThreadAlloc(KThreadProc proc, void* user_data) {
  if (!proc) {
    return NULL;
  }

  KThreadRef thread = KObjectAlloc(&KThreadClass);
  if (!thread) {
    return NULL;
  }

  // Will be released before main is called.
  KThreadRetain(thread);

  thread->main = proc;
  thread->user_data = user_data;

#if K_OS_WIN
  HANDLE thread_handle =
      CreateThread(NULL,          // LPSECURITY_ATTRIBUTES lpThreadAttributes
                   0,             // SIZE_T dwStackSize (0 is default)
                   &KThreadMain,  // LPTHREAD_START_ROUTINE lpStartAddress
                   thread,        // LPVOID lpParameter
                   0,             // DWORD dwCreationFlags
                   NULL           // LPDWORD lpThreadId
      );
  if (!thread_handle) {
    KThreadRelease(thread);  // main won't be called.
    KThreadRelease(thread);
    return NULL;
  }

  thread->handle = thread_handle;
#else   // K_OS_WIN
  pthread_t handle;
  if (pthread_create(&handle,       // handle out
                     NULL,          // attributes
                     &KThreadMain,  // start routine
                     thread         // arg
                     ) != 0) {
    KThreadRelease(thread);  // main won't be called.
    KThreadRelease(thread);
    return NULL;
  }
  thread->handle = handle;
  thread->is_valid = true;
#endif  // K_OS_WIN

  return thread;
}

void KThreadRetain(KThreadRef thread) {
  KObjectRetain(thread);
}

void KThreadRelease(KThreadRef thread) {
  KObjectRelease(thread);
}

void KThreadJoin(KThreadRef thread) {
  if (!thread) {
    return;
  }

#if K_OS_WIN
  if (thread->is_joined || !thread->handle) {
    return;
  }
  WaitForSingleObject(thread->handle, INFINITE);
  CloseHandle(thread->handle);
  thread->is_joined = true;
  thread->handle = NULL;
#else   // K_OS_WIN
  if (thread->is_joined || !thread->is_valid) {
    return;
  }
  pthread_join(thread->handle, NULL);
  thread->is_joined = true;
  thread->is_valid = false;
#endif  // K_OS_WIN
}

size_t KThreadGetHardwareConcurrency() {
#if K_OS_WIN
  // TODO
  return 1;
#else   // K_OS_WIN
  long result = sysconf(_SC_NPROCESSORS_CONF);
  if (result <= 0) {
    return 1;
  }
  return result;
#endif  // K_OS_WIN
}
