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

K_IMPL_OBJECT(KMapEntry);

static KMapEntryRef KMapEntryNew(KObjectRef key, KObjectRef value) {
  if (!key || !value) {
    return NULL;
  }
  KMapEntryRef entry = KMapEntryAlloc();
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
static const double kMapMaxLoadFactor = 1.0;
static const size_t kMapBucketGrowthFactor = 2;

struct KMap {
  KMapHash hash;
  KMapEqual equal;
  KArrayRef buckets;
  size_t object_count;
};

static void KMapInit(KMapRef map) {}

static void KMapDeInit(KMapRef map) {
  KArrayRelease(map->buckets);
}

K_IMPL_OBJECT(KMap);

KMapRef KMapAllocWithBucketCount(KMapHash hash,
                                 KMapEqual equal,
                                 size_t bucket_count) {
  if (!hash || !equal || bucket_count == 0) {
    return NULL;
  }

  KMapRef map = KMapAlloc();
  if (!map) {
    return NULL;
  }

  map->buckets = KArrayNew();
  if (!map->buckets) {
    KMapRelease(map);
    return NULL;
  }

  for (size_t i = 0; i < bucket_count; i++) {
    KArrayRef bucket = KArrayNew();
    if (!bucket) {
      KMapRelease(map);
      return NULL;
    }
    bool added = KArrayAddObject(map->buckets, bucket);
    KArrayRelease(bucket);
    if (!added) {
      KMapRelease(map);
      return NULL;
    }
  }

  map->hash = hash;
  map->equal = equal;

  return map;
}

KMapRef KMapNew(KMapHash hash, KMapEqual equal) {
  return KMapAllocWithBucketCount(hash, equal, kMapInitialBucketsSize);
}

static KArrayRef KMapGetBucket(KMapRef map, KObjectRef key) {
  if (!map || !key) {
    return NULL;
  }

  size_t hash = map->hash(key);
  size_t index = hash % KArrayGetLength(map->buckets);
  return KArrayGetObjectAtIndex(map->buckets, index);
}

static bool KMapRehashIfNecessary(KMapRef map) {
  if (KMapGetLoadFactor(map) < kMapMaxLoadFactor) {
    return true;
  }

  size_t new_bucket_count =
      KArrayGetLength(map->buckets) * kMapBucketGrowthFactor;

  KMapRef new_map =
      KMapAllocWithBucketCount(map->hash, map->equal, new_bucket_count);

  if (!new_map) {
    return false;
  }

  //----------------------------------------------------------------------------
  /// Add existing entries to the new map.
  ///
  size_t iterated = 0;
  for (size_t i = 0, bucket_count = KArrayGetLength(map->buckets);
       i < bucket_count; i++) {
    KArrayRef bucket = KArrayGetObjectAtIndex(map->buckets, i);
    K_ASSERT(bucket);
    for (size_t j = 0, entry_count = KArrayGetLength(bucket); j < entry_count;
         j++) {
      KMapEntryRef entry = KArrayGetObjectAtIndex(bucket, j);
      K_ASSERT(entry);
      if (!KMapSetValue(new_map, entry->key, entry->value)) {
        KMapRelease(new_map);
        return false;
      }
      iterated++;
    }
  }

  K_ASSERT(KMapGetCount(map) == KMapGetCount(new_map));

  //----------------------------------------------------------------------------
  /// Swap buckets with the new map.
  ///
  KArrayRef temp = new_map->buckets;
  new_map->buckets = map->buckets;
  map->buckets = temp;

  KMapRelease(new_map);

  return true;
}

static bool KMapSetValueNoRehash(KMapRef map,
                                 KObjectRef key,
                                 KObjectRef value) {
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
  KMapEntryRef new_entry = KMapEntryNew(key, value);
  bool added = KArrayAddObject(bucket, new_entry);
  KMapEntryRelease(new_entry);
  if (added) {
    map->object_count++;
    return true;
  }

  return false;
}

bool KMapSetValue(KMapRef map, KObjectRef key, KObjectRef value) {
  if (!KMapSetValueNoRehash(map, key, value)) {
    return false;
  }
  KMapRehashIfNecessary(map);
  return true;
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

size_t KMapGetMaxBucketUtilization(KMapRef map) {
  if (!map) {
    return 0u;
  }
  size_t utiliation = 0u;
  for (size_t i = 0, count = KArrayGetLength(map->buckets); i < count; i++) {
    size_t bucket_count =
        KArrayGetLength(KArrayGetObjectAtIndex(map->buckets, i));
    if (bucket_count > utiliation) {
      utiliation = bucket_count;
    }
  }
  return utiliation;
}
