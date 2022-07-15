#pragma once

#include "kfile_path.h"
#include "kmacros.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KFileHandle);

KFileHandleRef KFileHandleNew(KFilePathRef string);

K_EXTERN_C_END
