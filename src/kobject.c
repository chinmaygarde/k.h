#include "kobject.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct KControlBlock {
  const KClass* isa;
  _Atomic(uint32_t) ref_count;
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
  atomic_fetch_add(&KCONTROL_BLOCK->ref_count, 1u);
}

void KObjectRelease(KObjectRef obj) {
  if (obj == NULL) {
    return;
  }
  if (atomic_fetch_sub(&KCONTROL_BLOCK->ref_count, 1u) == 1) {
    KCONTROL_BLOCK->isa->deinit(obj);
    free(KCONTROL_BLOCK);
  }
}

size_t KObjectGetRetainCount(KObjectRef obj) {
  if (obj == NULL) {
    return 0u;
  }
  return atomic_load(&KCONTROL_BLOCK->ref_count);
}
