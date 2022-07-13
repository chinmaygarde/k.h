#include "kmap.h"

#include "karray.h"
#include "kassert.h"

//------------------------------------------------------------------------------
/// KMapEntry Implementation
///

K_DEF_OBJECT(KMapEntry);

struct KMapEntry {
  KObjectRef key;
  KObjectRef value;
};

static void KMapEntryInit(KMapEntryRef entry) {}

static void KMapEntryDeInit(KMapEntryRef entry) {
  KObjectRelease(entry->key);
  KObjectRelease(entry->value);
}

static KMapEntryRef KMapEntryAlloc(KObjectRef key, KObjectRef value) {
  if (!key || !value) {
    return NULL;
  }
  static KClass kMapEntryClass = {
      .init = (KClassInit)&KMapEntryInit,
      .deinit = (KClassDeinit)&KMapEntryDeInit,
      .size = sizeof(struct KMapEntry),
  };
  KMapEntryRef entry = KObjectAlloc(&kMapEntryClass);
  if (!entry) {
    return NULL;
  }
  KObjectRetain(key);
  KObjectRetain(value);
  entry->key = key;
  entry->value = value;
  return entry;
}

//------------------------------------------------------------------------------
/// KMap Implementation
///
static const size_t kMapInitialBucketsSize = 16u;

struct KMap {
  KMapHash hash;
  KMapEqual equal;
  KArrayRef buckets;
  bool is_valid;
  size_t object_count;
};

void KMapInit(KMapRef map) {
  map->buckets = KArrayAlloc();
  if (!map->buckets) {
    return;
  }
  for (size_t i = 0; i < kMapInitialBucketsSize; i++) {
    KArrayRef bucket = KArrayAlloc();
    if (!bucket) {
      return;
    }
    bool added = KArrayAddObject(map->buckets, bucket);
    KArrayRelease(bucket);
    if (!added) {
      return;
    }
  }
  map->is_valid = true;
}

void KMapDeInit(KMapRef map) {
  KArrayRelease(map->buckets);
}

static KClass KMapClass = {
    .init = (KClassInit)&KMapInit,
    .deinit = (KClassDeinit)&KMapDeInit,
    .size = sizeof(struct KMap),
};

KMapRef KMapAlloc(KMapHash hash, KMapEqual equal) {
  if (!hash || !equal) {
    return NULL;
  }

  KMapRef map = KObjectAlloc(&KMapClass);
  if (!map) {
    return NULL;
  }

  if (!map->is_valid) {
    KMapRelease(map);
    return NULL;
  }

  map->hash = hash;
  map->equal = equal;

  return map;
}

static KArrayRef KMapGetBucket(KMapRef map, KObjectRef key) {
  if (!map || !key) {
    return NULL;
  }

  size_t hash = map->hash(key);
  size_t index = hash % KArrayGetLength(map->buckets);
  return KArrayGetObjectAtIndex(map->buckets, index);
}

bool KMapSetValue(KMapRef map, KObjectRef key, KObjectRef value) {
  if (!value) {
    return false;
  }

  KArrayRef bucket = KMapGetBucket(map, key);
  if (!bucket) {
    return false;
  }

  // Look for collisions and update in place. No updates to count.
  for (size_t i = 0, count = KArrayGetLength(bucket); i < count; i++) {
    KMapEntryRef entry = KArrayGetObjectAtIndex(bucket, i);
    K_ASSERT(entry != NULL);
    if (map->equal(key, entry->key)) {
      if (value == entry->value) {
        // Already in the map.
        return true;
      }
      // Update the value.
      KObjectRelease(entry->value);
      KObjectRetain(value);
      entry->value = value;
      return true;
    }
  }

  // No collisions. Update count.
  KMapEntryRef new_entry = KMapEntryAlloc(key, value);
  bool added = KArrayAddObject(bucket, new_entry);
  KMapEntryRelease(new_entry);
  if (added) {
    map->object_count++;
    return true;
  }

  return false;
}

KObjectRef KMapGetValue(KMapRef map, KObjectRef key) {
  KArrayRef bucket = KMapGetBucket(map, key);
  if (!bucket) {
    return NULL;
  }
  for (size_t i = 0, count = KArrayGetLength(bucket); i < count; i++) {
    KMapEntryRef entry = KArrayGetObjectAtIndex(bucket, i);
    if (map->equal(key, entry->key)) {
      return entry->value;
    }
  }
  return NULL;
}

bool KMapRemoveValue(KMapRef map, KObjectRef key) {
  KArrayRef bucket = KMapGetBucket(map, key);
  if (!bucket) {
    return false;
  }
  for (size_t i = 0, count = KArrayGetLength(bucket); i < count; i++) {
    KMapEntryRef entry = KArrayGetObjectAtIndex(bucket, i);
    if (map->equal(key, entry->key)) {
      if (KArrayRemoveObjectAtIndex(bucket, i)) {
        map->object_count--;
        return true;
      }
    }
  }
  return false;
}

size_t KMapGetCount(KMapRef map) {
  if (!map) {
    return 0u;
  }
  return map->object_count;
}

double KMapGetLoadFactor(KMapRef map) {
  if (!map) {
    return 0u;
  }
  return (double)map->object_count / KArrayGetLength(map->buckets);
}
