#include "klist.h"

#include <stdlib.h>

#include "kassert.h"

typedef struct KListEntry {
  struct KListEntry* prev;
  struct KListEntry* next;
  KObjectRef item;
} KListEntry;

struct KList {
  KListEntry* root;
  size_t count;
};

K_IMPL_OBJECT(KList);

void KListInit(KListRef list) {}

void KListDeInit(KListRef list) {
  size_t count = KListGetCount(list);
  size_t removed = KListRemoveAllObjects(list);
  K_ASSERT(count == removed);
}

KListRef KListNew() {
  KListRef list = KListAlloc();
  if (!list) {
    return NULL;
  }
  return list;
}

bool KListAddObject(KListRef list, KObjectRef obj) {
  if (!list || !obj) {
    return false;
  }

  KListEntry* new_entry = calloc(1u, sizeof(KListEntry));
  if (!new_entry) {
    return false;
  }

  KObjectRetain(obj);
  new_entry->item = obj;

  if (!list->root) {
    list->root = new_entry;
  } else {
    KListEntry* entry = list->root;
    while (entry->next != NULL) {
      entry = entry->next;
    }
    K_DASSERT(entry->next == NULL);
    entry->next = new_entry;
    new_entry->prev = entry;
  }

  list->count++;
  return true;
}

KObjectRef KListRemoveObjectAtIndex(KListRef list, size_t index) {
  if (!list) {
    return NULL;
  }
  size_t i = 0;
  for (KListEntry* entry = list->root; entry != NULL; entry = entry->next) {
    if (i == index) {
      KListEntry* prev = entry->prev;
      KListEntry* next = entry->next;
      if (prev) {
        prev->next = next;
      }
      if (next) {
        next->prev = prev;
      }
      KObjectRef item = entry->item;
      if (i == 0) {
        list->root = next;
      }
      free(entry);
      list->count--;
      return item;
    }
    i++;
  }
  return NULL;
}

KObjectRef KListGetObjectAtIndex(KListRef list, size_t index) {
  if (!list) {
    return NULL;
  }
  size_t i = 0;
  for (KListEntry* entry = list->root; entry != NULL; entry = entry->next) {
    if (i == index) {
      return entry->item;
    }
    i++;
  }
  return NULL;
}

size_t KListIterate(KListRef list, KListIterator iterator, void* user_data) {
  if (!list || !iterator) {
    return 0;
  }
  size_t i = 0;
  for (KListEntry* entry = list->root; entry != NULL; entry = entry->next) {
    i++;
    if (!iterator(entry->item, i, user_data)) {
      break;
    }
  }
  return i;
}

bool KListContainsObject(KListRef list, KObjectRef obj) {
  if (!list || !obj) {
    return false;
  }
  for (KListEntry* entry = list->root; entry != NULL; entry = entry->next) {
    if (entry->item == obj) {
      return true;
    }
  }
  return false;
}

size_t KListGetCount(KListRef list) {
  if (!list) {
    return 0u;
  }
  return list->count;
}

size_t KListRemoveAllObjects(KListRef list) {
  if (!list) {
    return 0u;
  }

  size_t removed = 0u;

  for (KListEntry* entry = list->root; entry != NULL;) {
    KListEntry* current = entry;
    entry = entry->next;
    KObjectRelease(current->item);
    free(current);
    removed++;
  }
  list->root = NULL;
  K_ASSERT(removed == list->count);
  list->count = 0u;
  return removed;
}
