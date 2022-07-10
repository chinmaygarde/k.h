#include "kallocation.h"

#include <stdlib.h>

#include "kobject.h"

struct KAllocation {
  size_t size;
  void* buffer;
};

static void KAllocationInit(KAllocationRef allocation) {}

static void KAllocationDeInit(KAllocationRef allocation) {
  free(allocation->buffer);
}

static KClass kAllocationClass = {.init = (KClassInit)&KAllocationInit,
                                  .deinit = (KClassDeinit)&KAllocationDeInit,
                                  .size = sizeof(struct KAllocation)};

KAllocationRef KAllocationAlloc() {
  return KObjectAlloc(&kAllocationClass);
}

void KAllocationRetain(KAllocationRef allocation) {
  KObjectRetain(allocation);
}

void KAllocationRelease(KAllocationRef allocation) {
  KObjectRelease(allocation);
}

bool KAllocationTruncate(KAllocationRef allocation, size_t size) {
  if (!allocation) {
    return false;
  }

  void* buffer = realloc(allocation->buffer, size);

  if (buffer == NULL) {
    // Reallocation failure. Original buffer intact.
    return false;
  }

  allocation->buffer = buffer;
  allocation->size = size;

  return true;
}

size_t KAllocationGetSize(KAllocationRef allocation) {
  if (!allocation) {
    return 0u;
  }
  return allocation->size;
}

void* KAllocationGetBuffer(KAllocationRef allocation) {
  return allocation->buffer;
}

uint32_t KAllocationNextPowerOfTwoSize(uint32_t x) {
  if (x == 0) {
    return 1;
  }

  --x;

  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;

  return x + 1;
}
