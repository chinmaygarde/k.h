#pragma once

#include "kmacros.h"
#include "kobject.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KMapping);

KMappingRef KMappingNew(void* mapping, size_t count, KObjectRef user_data);

void* KMappingGetBuffer(KMappingRef mapping);

size_t KMappingGetSize(KMappingRef mapping);

KObjectRef KMappingGetUserData(KMappingRef mapping);

K_EXTERN_C_END
