#include "kset.h"

#include "kassert.h"

struct KSet {
  KSetHash hash;
  KSetEqual equal;
  size_t count;
  bool valid;
};

K_IMPL_OBJECT(KSet);

// static const size_t kSetInitialBucketsSize = 16u;
// static const double kSetMaxLoadFactor = 1.0;
// static const size_t kSetBucketGrowthFactor = 2;

void KSetInit(KSetRef set) {}

void KSetDeInit(KSetRef set) {}

KSetRef KSetNew(KSetHash hash, KSetEqual equal) {
  if (hash == NULL || equal == NULL) {
    return NULL;
  }
  KSetRef set = KSetAlloc();
  if (!set) {
    return NULL;
  }
  if (!set->valid) {
    KSetRelease(set);
    return NULL;
  }
  set->hash = hash;
  set->equal = equal;
  return set;
}

bool KSetInsert(KSetRef set, KObjectRef object) {
  KAbort();
}

bool KSetErase(KSetRef set, KObjectRef object) {
  KAbort();
}

bool KSetContains(KSetRef set, KObjectRef object) {
  KAbort();
}

size_t KSetGetCount(KSetRef set) {
  KAbort();
}

bool KSetClear(KSetRef set) {
  KAbort();
}
