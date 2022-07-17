#pragma once

#include "kfile_path.h"
#include "kmacros.h"
#include "kmapping.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KFileHandle);

typedef enum {
  kFilePermissionReadOnly,
  kFilePermissionWriteOnly,
  kFilePermissionReadWrite,
} KFilePermission;

typedef enum {
  kMapProtectionNone = 1 << 0,
  kMapProtectionRead = 1 << 1,
  kMapProtectionWrite = 1 << 2,
  kMapProtectionExec = 1 << 3,
} KMapProtection;

KFileHandleRef KFileHandleNew(KFilePathRef string, KFilePermission permission);

bool KFileHandleClose(KFileHandleRef handle);

bool KFileHandleGetSize(KFileHandleRef handle, size_t* size_out);

KMappingRef KFileHandleNewMapping(KFileHandleRef handle,
                                  size_t size,
                                  KMapProtection prot);

K_EXTERN_C_END
