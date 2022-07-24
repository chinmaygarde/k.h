#include "kfile_handle.h"

#include "kassert.h"
#include "kplatform.h"

#if K_OS_WIN

#include <Windows.h>

#else  // K_OS_WIN

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#endif  // K_OS_WIN

struct KFileHandle {
#if K_OS_WIN
  HANDLE handle;
#else   // K_OS_WIN
  int handle;
#endif  // K_OS_WIN
  bool closed;
};

K_IMPL_OBJECT(KFileHandle);

void KFileHandleInit(KFileHandleRef handle) {}

void KFileHandleDeInit(KFileHandleRef handle) {
  K_ASSERT(KFileHandleClose(handle));
}

KFileHandleRef KFileHandleNew(KFilePathRef path, KFilePermission permission) {
  if (!path) {
    return NULL;
  }
  KStringRef string = KFilePathNewStringRepresentation(path);
  if (!string) {
    return NULL;
  }

#if K_OS_WIN
  DWORD desired_access = 0;
  switch (permission) {
    case kFilePermissionReadOnly:
      desired_access |= GENERIC_READ;
      break;
    case kFilePermissionWriteOnly:
      desired_access |= GENERIC_WRITE;
      break;
    case kFilePermissionReadWrite:
      desired_access |= GENERIC_READ;
      desired_access |= GENERIC_WRITE;
      break;
  }
  HANDLE raw_handle =
      CreateFile(KStringGetData(string),  // LPCSTR lpFileName
                 desired_access,  // DWORD                 dwDesiredAccess
                 0,               // DWORD                 dwShareMode
                 NULL,            // LPSECURITY_ATTRIBUTES lpSecurityAttributes
                 OPEN_EXISTING,   // DWORD                 dwCreationDisposition
                 FILE_ATTRIBUTE_NORMAL,  // DWORD dwFlagsAndAttributes
                 NULL                    // HANDLE                hTemplateFile
      );
  KStringRelease(string);
  if (raw_handle == INVALID_HANDLE_VALUE) {
    return NULL;
  }
  KFileHandleRef handle = KFileHandleAlloc();
  handle->handle = raw_handle;
  return handle;
#else   // K_OS_WIN
  int flags = O_CLOEXEC;
  switch (permission) {
    case kFilePermissionReadOnly:
      flags |= O_RDONLY;
      break;
    case kFilePermissionWriteOnly:
      flags |= O_WRONLY;
      break;
    case kFilePermissionReadWrite:
      flags |= O_RDWR;
      break;
  }
  int raw_handle = K_HANDLE_EINTR(open(KStringGetData(string), flags));
  KStringRelease(string);
  if (raw_handle == -1) {
    return NULL;
  }
  KFileHandleRef handle = KFileHandleAlloc();
  handle->handle = raw_handle;
  return handle;
#endif  // K_OS_WIN
}

bool KFileHandleClose(KFileHandleRef handle) {
  if (!handle) {
    return false;
  }
  if (handle->closed) {
    return true;
  }
#if K_OS_WIN
  handle->closed = CloseHandle(handle->handle);
  return handle->closed;
#else   // K_OS_WIN
  handle->closed = K_HANDLE_EINTR(close(handle->handle)) == 0;
  return handle->closed;
#endif  // K_OS_WIN
}

bool KFileHandleGetSize(KFileHandleRef handle, size_t* size_out) {
  if (!handle || handle->closed || !size_out) {
    return false;
  }

#if K_OS_WIN
  DWORD file_size = GetFileSize(handle->handle, NULL);
  if (file_size == INVALID_FILE_SIZE) {
    return false;
  }
  *size_out = file_size;
  return true;
#else   // K_OS_WIN
  struct stat stat_buffer;
  if (fstat(handle->handle, &stat_buffer) != 0) {
    return false;
  }
  *size_out = stat_buffer.st_size;
  return true;
#endif  // K_OS_WIN
}

//------------------------------------------------------------------------------
/// File Mapping
///
K_DEF_OBJECT(__KFileMapping);

struct __KFileMapping {
#if K_OS_WIN
  HANDLE handle;
  LPVOID mapping;
  bool valid;
#else   // K_OS_WIN
  void* mapping;
  size_t size;
#endif  // K_OS_WIN
};

K_IMPL_OBJECT(__KFileMapping);

void __KFileMappingInit(__KFileMappingRef mapping) {}

void __KFileMappingDeInit(__KFileMappingRef mapping) {
#if K_OS_WIN
  if (!mapping->valid) {
    return;
  }
  K_ASSERT(UnmapViewOfFile(mapping->mapping) == TRUE);
  K_ASSERT(CloseHandle(mapping->handle) == TRUE);
#else   // K_OS_WIN
  munmap(mapping->mapping, mapping->size);
#endif  // K_OS_WIN
}

KMappingRef KFileHandleNewMapping(KFileHandleRef handle,
                                  size_t size,
                                  KMapProtection protections) {
  if (!handle || handle->closed) {
    return NULL;
  }

#if K_OS_WIN
  DWORD prot = 0;
  DWORD access = 0;
  if (protections & kMapProtectionNone) {
    prot = 0;
  }
  if (protections & kMapProtectionRead) {
    prot |= PAGE_READONLY;
    access |= FILE_MAP_READ;
  }
  if (protections & kMapProtectionWrite) {
    prot |= PAGE_READWRITE;
    access |= FILE_MAP_WRITE;
  }
  if (protections & kMapProtectionExec) {
    prot |= PAGE_EXECUTE_READWRITE;
    access |= FILE_MAP_ALL_ACCESS;
  }
  HANDLE map_handle =
      CreateFileMapping(handle->handle,  // HANDLE                hFile
                        NULL,  // LPSECURITY_ATTRIBUTES lpFileMappingAttributes
                        prot,  // DWORD                 flProtect
                        0,     // DWORD                 dwMaximumSizeHigh
                        0,     // DWORD                 dwMaximumSizeLow
                        NULL   // LPCSTR                lpName
      );
  if (map_handle == NULL) {
    return NULL;
  }

  LPVOID mapping = MapViewOfFile(map_handle,  // HANDLE hFileMappingObject
                                 access,      // DWORD  dwDesiredAccess
                                 0,           // DWORD  dwFileOffsetHigh
                                 0,           // DWORD  dwFileOffsetLow
                                 size         // SIZE_T dwNumberOfBytesToMap
  );

  if (mapping == NULL) {
    K_ASSERT(CloseHandle(map_handle) == TRUE);
    return NULL;
  }
  __KFileMappingRef file_mapping = __KFileMappingAlloc();
  K_ASSERT(file_mapping);
  file_mapping->handle = map_handle;
  file_mapping->mapping = mapping;
  file_mapping->valid = true;
  KMappingRef result = KMappingNew(file_mapping->mapping, size, file_mapping);
  __KFileMappingRelease(file_mapping);
  return result;
#else   // K_OS_WIN
  int prot = 0;
  if (protections & kMapProtectionNone) {
    prot |= PROT_NONE;
  }
  if (protections & kMapProtectionRead) {
    prot |= PROT_READ;
  }
  if (protections & kMapProtectionWrite) {
    prot |= PROT_WRITE;
  }
  if (protections & kMapProtectionExec) {
    prot |= PROT_EXEC;
  }

  void* mapping =
      mmap(NULL, size, prot, MAP_FILE | MAP_PRIVATE, handle->handle, 0);
  if (mapping == MAP_FAILED) {
    return NULL;
  }
  __KFileMappingRef file_mapping = __KFileMappingAlloc();
  K_ASSERT(file_mapping);
  file_mapping->mapping = mapping;
  file_mapping->size = size;
  KMappingRef result =
      KMappingNew(file_mapping->mapping, file_mapping->size, file_mapping);
  __KFileMappingRelease(file_mapping);
  return result;
#endif  // K_OS_WIN
}
