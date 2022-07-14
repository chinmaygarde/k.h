#pragma once

#include "kmacros.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

typedef void* KObjectRef;

typedef void (*KClassInit)(void*);
typedef void (*KClassDeinit)(void*);

typedef struct KClass {
  KClassInit init;
  KClassDeinit deinit;
  size_t size;
} KClass;

struct KControlBlock;

KObjectRef KObjectNew(const KClass* clasz);

void KObjectRetain(KObjectRef obj);

void KObjectRelease(KObjectRef obj);

size_t KObjectGetRetainCount(KObjectRef obj);

K_EXTERN_C_END
