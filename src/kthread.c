#include "kthread.h"

#include <Windows.h>
#include "klogging.h"
#include "ktypes.h"

struct KThread {
  bool is_joined;
  KThreadProc main;
  void* user_data;
  HANDLE handle;
};

static void KThreadInit(KThreadRef thread) {}

static void KThreadDeInit(KThreadRef thread) {
  KThreadJoin(thread);
}

static KClass KThreadClass = {
    .init = (KClassInit)&KThreadInit,
    .deinit = (KClassDeinit)&KThreadDeInit,
    .size = sizeof(struct KThread),
};

static DWORD KThreadMain(LPVOID arg) {
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

  if (thread->is_joined || !thread->handle) {
    return;
  }

  WaitForSingleObject(thread->handle, INFINITE);
  CloseHandle(thread->handle);
  thread->is_joined = true;
  thread->handle = NULL;
}
