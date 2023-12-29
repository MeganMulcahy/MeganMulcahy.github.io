//table.c
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void createTable(Table* table) {
  table->tableCount = 0;
  table->tableCapacity = 0;
  table->tableEntries = NULL;
}

void deallocTable(Table* table) {
  FREE_ARRAY(Entry, table->tableEntries, table->tableCapacity);
  createTable(table);
}

static Entry* findEntry(Entry* tableEntries, int tableCapacity, ObjString* entryKey) {
  uint32_t index = entryKey->hash & (tableCapacity - 1);
  Entry* tombstone = NULL;
  for (;;) {
    Entry* entry = &tableEntries[index];
    if (entry->entryKey == NULL) {
      if (IS_NIL(entry->entryValue)) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (entry->entryKey == entryKey) {
      return entry;
    }
    index = (index + 1) & (tableCapacity - 1);
  }
}

bool findTableEntry(Table* table, ObjString* entryKey, Value* entryValue) {
  if (table->tableCount == 0) return false;

  Entry* entry = findEntry(table->tableEntries, table->tableCapacity, entryKey);
  if (entry->entryKey == NULL) return false;

  *entryValue = entry->entryValue;
  return true;
}

static void adjustCapacity(Table* table, int tableCapacity) {
  Entry* tableEntries = ALLOCATE(Entry, tableCapacity);
  for (int i = 0; i < tableCapacity; i++) {
    tableEntries[i].entryKey = NULL;
    tableEntries[i].entryValue = NIL_VAL;
  }

  table->tableCount = 0;
  int i = 0;
  while (i < table->tableCapacity) {
      Entry* entry = &table->tableEntries[i];
      if (entry->entryKey != NULL) {
          Entry* dest = findEntry(tableEntries, tableCapacity, entry->entryKey);
          dest->entryKey = entry->entryKey;
          dest->entryValue = entry->entryValue;
          table->tableCount++;
      }
      i++;
  }


  FREE_ARRAY(Entry, table->tableEntries, table->tableCapacity);
  table->tableEntries = tableEntries;
  table->tableCapacity = tableCapacity;
}

bool setTableEntry(Table* table, ObjString* entryKey, Value entryValue) {
  if (table->tableCount + 1 > table->tableCapacity * TABLE_MAX_LOAD) {
    int tableCapacity = GROW_CAPACITY(table->tableCapacity);
    adjustCapacity(table, tableCapacity);
  }

  Entry* entry = findEntry(table->tableEntries, table->tableCapacity, entryKey);
  bool isNewKey = entry->entryKey == NULL;
  if (isNewKey && IS_NIL(entry->entryValue)) table->tableCount++;

  entry->entryKey = entryKey;
  entry->entryValue = entryValue;
  return isNewKey;
}

bool deleteTableEntry(Table* table, ObjString* entryKey) {
  if (table->tableCount == 0) return false;
  Entry* entry = findEntry(table->tableEntries, table->tableCapacity, entryKey);
  if (entry->entryKey == NULL) return false;
  entry->entryKey = NULL;
  entry->entryValue = BOOL_VAL(true);
  return true;
}

void mergeTables(Table* from, Table* to) {
  int i = 0;
  while (i < from->tableCapacity) {
    Entry* entry = &from->tableEntries[i];
    if (entry->entryKey != NULL) {
        setTableEntry(to, entry->entryKey, entry->entryValue);
    }
    i++;
  }

}

ObjString* findStringInTable(Table* table, const char* chars, int length, uint32_t hash) {
  if (table->tableCount == 0) return NULL;
  uint32_t index = hash & (table->tableCapacity - 1);
  while (1) {
    Entry* entry = &table->tableEntries[index];
    if (entry->entryKey == NULL) {
        if (IS_NIL(entry->entryValue)) return NULL;
    } else if (entry->entryKey->stringLength == length &&
               entry->entryKey->hash == hash &&
               memcmp(entry->entryKey->chars, chars, length) == 0) {
        return entry->entryKey;
    }

    index = (index + 1) & (table->tableCapacity - 1);
  }

}

void removeEmptyEntries(Table* table) {
  int i = 0;
  while (i < table->tableCapacity) {
      Entry* entry = &table->tableEntries[i];
      if (entry->entryKey != NULL && !entry->entryKey->obj.isMarked) {
          deleteTableEntry(table, entry->entryKey);
      }
      i++;
  }
}

void markReferencesInTable(Table* table) {
  for (int i = 0; i < table->tableCapacity; i++) {
    Entry* entry = &table->tableEntries[i];
    indicateObject((Obj*)entry->entryKey);
    indicateValue(entry->entryValue);
  }
}
