#pragma once

#include "kfile_path.h"
#include "kmacros.h"
#include "kmapping.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// An OS specific handle to a file.
///
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

//------------------------------------------------------------------------------
/// @brief      Create or open a new file handle at the given path.
///
/// @param[in]  path        The path.
/// @param[in]  permission  The permission
///
/// @return     The new file handle reference.
///
KFileHandleRef KFileHandleNew(KFilePathRef path, KFilePermission permission);

//------------------------------------------------------------------------------
/// @brief      Close the file handle explicitly. File handles are usually
///             closed when the final reference is dropped.
///
/// @param[in]  handle  The handle.
///
/// @return     If the handle was successfully closed.
///
bool KFileHandleClose(KFileHandleRef handle);

//------------------------------------------------------------------------------
/// @brief      Get the file size of the file pointed to by the handle.
///
/// @param[in]  handle    The handle.
/// @param[out] size_out  The file size on success.
///
/// @return     If the file size was successfully returned in the `size_out`
///             parameter.
///
bool KFileHandleGetSize(KFileHandleRef handle, size_t* size_out);

//------------------------------------------------------------------------------
/// @brief      Create a mapping containing the contents of the file pointed to
///             by the open handle.
///
/// @param[in]  handle  The handle.
/// @param[in]  size    The size.
/// @param[in]  prot    The mapping protections.
///
/// @return     The new mapping reference.
///
KMappingRef KFileHandleNewMapping(KFileHandleRef handle,
                                  size_t size,
                                  KMapProtection prot);

K_EXTERN_C_END
