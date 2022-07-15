#pragma once

#include "kmacros.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KFilePath);

K_PRINTF_FORMAT(1, 2)
KFilePathRef KFilePathNewWithFormat(const char* format, ...);

KFilePathRef KFilePathNewWithString(KStringRef path);

bool KFilePathAddComponent(KFilePathRef path, KFilePathRef comp);

bool KFilePathIsAbsolute(KFilePathRef path);

size_t KFilePathCountComponents(KFilePathRef path);

KStringRef KFilePathNewStringRepresentation(KFilePathRef path);

KStringRef KFilePathNewStringRepresentationWithSeparator(KFilePathRef path,
                                                         char sep);

K_EXTERN_C_END
