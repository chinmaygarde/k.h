#include "kmap.h"

#include <stdbool.h>

#include "karray.h"

struct KMap {
  KMapKeyHash hash;
  KMapKeyEqual equal;
  KArrayRef buckets;
  bool is_valid;
};

static const size_t kInitialBucketsSize = 16u;

void KMapInit(KMapRef map) {
  map->buckets = KArrayAlloc();
  // if (!KArrayResize(NULL, kInitialBucketsSize)) {
  //   return;
  // }
  map->is_valid = true;
}

void KMapDeinit(KMapRef map) {
  KArrayRelease(map->buckets);
}

static KClass kMapClass = {
    .init = (KClassInit)&KMapInit,
    .deinit = (KClassDeinit)&KMapDeinit,
    .size = sizeof(struct KMap),
};

KMapRef KMapAlloc(KMapKeyHash hash, KMapKeyEqual equal) {
  if (!hash || !equal) {
    return NULL;
  }
  KMapRef map = KObjectAlloc(&kMapClass);
  if (!map || !map->is_valid) {
    return NULL;
  }
  map->hash = hash;
  map->equal = equal;
  return map;
}

void KMapRetain(KMapRef map) {
  KObjectRetain(map);
}

void KMapRelease(KMapRef map) {
  KObjectRelease(map);
}
