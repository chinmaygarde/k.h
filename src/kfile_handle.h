#pragma once

#include "kfile_path.h"
#include "kmacros.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KFileHandle);

typedef enum {
  kFilePermissionReadOnly,
  kFilePermissionWriteOnly,
  kFilePermissionReadWrite,
} KFilePermission;

KFileHandleRef KFileHandleNew(KFilePathRef string, KFilePermission permission);

bool KFileHandleClose(KFileHandleRef handle);

K_EXTERN_C_END
