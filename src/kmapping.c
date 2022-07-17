#include "kmapping.h"

struct KMapping {
  void* mapping;
  size_t count;
  KObjectRef user_data;
};

K_IMPL_OBJECT(KMapping);

void KMappingInit(KMappingRef mapping) {}

void KMappingDeInit(KMappingRef mapping) {
  KObjectRelease(mapping->user_data);
}

KMappingRef KMappingNew(void* buffer, size_t count, KObjectRef user_data) {
  KMappingRef mapping = KMappingAlloc();
  if (!mapping) {
    return NULL;
  }
  KObjectRetain(user_data);
  mapping->mapping = buffer;
  mapping->count = count;
  mapping->user_data = user_data;
  return mapping;
}

void* KMappingGetBuffer(KMappingRef mapping) {
  if (!mapping) {
    return NULL;
  }
  return mapping->mapping;
}

size_t KMappingGetSize(KMappingRef mapping) {
  if (!mapping) {
    return 0u;
  }
  return mapping->count;
}

KObjectRef KMappingGetUserData(KMappingRef mapping) {
  if (!mapping) {
    return NULL;
  }
  return mapping->user_data;
}
