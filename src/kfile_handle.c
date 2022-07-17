#include "kfile_handle.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "kassert.h"

struct KFileHandle {
  int handle;
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
}

bool KFileHandleClose(KFileHandleRef handle) {
  if (!handle) {
    return false;
  }
  if (handle->closed) {
    return true;
  }
  handle->closed = K_HANDLE_EINTR(close(handle->handle)) == 0;
  return handle->closed;
}

bool KFileHandleGetSize(KFileHandleRef handle, size_t* size_out) {
  if (!handle || handle->closed || !size_out) {
    return false;
  }
  struct stat stat_buffer;
  if (fstat(handle->handle, &stat_buffer) != 0) {
    return false;
  }
  *size_out = stat_buffer.st_size;
  return true;
}

K_DEF_OBJECT(KPosixMapping);

struct KPosixMapping {
  void* mapping;
  size_t size;
};

K_IMPL_OBJECT(KPosixMapping);

void KPosixMappingInit(KPosixMappingRef mapping) {}

void KPosixMappingDeInit(KPosixMappingRef mapping) {
  munmap(mapping->mapping, mapping->size);
}

KMappingRef KFileHandleNewMapping(KFileHandleRef handle,
                                  size_t size,
                                  KMapProtection protections) {
  if (!handle || handle->closed) {
    return NULL;
  }

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
  KPosixMappingRef posix_mapping = KPosixMappingAlloc();
  K_ASSERT(posix_mapping);
  posix_mapping->mapping = mapping;
  posix_mapping->size = size;
  KMappingRef result =
      KMappingNew(posix_mapping->mapping, posix_mapping->size, posix_mapping);
  KPosixMappingRelease(posix_mapping);
  return result;
}
