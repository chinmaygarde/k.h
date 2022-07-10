#include "kobject.h"

#include <stdlib.h>

#include "kplatform.h"
#include "ktypes.h"

#if K_OS_WIN
#include <Windows.h>
#else  //  K_OS_WIN
#include <stdatomic.h>
#endif  // K_OS_WIN

typedef struct KControlBlock {
  const KClass* isa;
#if K_OS_WIN
  volatile LONG ref_count;
#else   // K_OS_WIN
  _Atomic(uint32_t) ref_count;
#endif  // K_OS_WIN
} KControlBlock;

KObjectRef KObjectAlloc(const KClass* clasz) {
  void* allocation = calloc(1u, sizeof(KControlBlock) + clasz->size);
  if (allocation == NULL) {
    return NULL;
  }
  // The control block comes first.
  KControlBlock* cb = (KControlBlock*)allocation;
  cb->isa = clasz;
  cb->ref_count = 1u;

  // Then the object itself.
  KObjectRef object =
      (KObjectRef)(((uint8_t*)allocation) + sizeof(KControlBlock));

  // Call the initializer.
  clasz->init(object);

  return object;
}

#define KCONTROL_BLOCK \
  ((KControlBlock*)(((uint8_t*)obj) - sizeof(KControlBlock)))

void KObjectRetain(KObjectRef obj) {
  if (obj == NULL) {
    return;
  }
#if K_OS_WIN
  InterlockedIncrement(&KCONTROL_BLOCK->ref_count);
#else   // K_OS_WIN
  atomic_fetch_add(&KCONTROL_BLOCK->ref_count, 1u);
#endif  // K_OS_WIN
}

void KObjectRelease(KObjectRef obj) {
  if (obj == NULL) {
    return;
  }

#if K_OS_WIN
  if (InterlockedDecrement(&KCONTROL_BLOCK->ref_count) == 0)
#else   // K_OS_WIN
  if (atomic_fetch_sub(&KCONTROL_BLOCK->ref_count, 1u) == 1)
#endif  // K_OS_WIN
  {
    KCONTROL_BLOCK->isa->deinit(obj);
    free(KCONTROL_BLOCK);
  }
}

size_t KObjectGetRetainCount(KObjectRef obj) {
  if (obj == NULL) {
    return 0u;
  }

#if K_OS_WIN
  return KCONTROL_BLOCK->ref_count;
#else   // K_OS_WIN
  return atomic_load(&KCONTROL_BLOCK->ref_count);
#endif  // K_OS_WIN
}
