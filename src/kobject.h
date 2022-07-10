#pragma once

#include "kmacros.h"

KEXTERN_C_BEGIN

typedef void* KObjectRef;

typedef void (*KClassInit)(void*);
typedef void (*KClassDeinit)(void*);

typedef struct KClass {
  KClassInit init;
  KClassDeinit deinit;
  size_t size;
} KClass;

struct KControlBlock;

KObjectRef KObjectAlloc(const KClass* clasz);

void KObjectRetain(KObjectRef obj);

void KObjectRelease(KObjectRef obj);

size_t KObjectGetRetainCount(KObjectRef obj);

KEXTERN_C_END
