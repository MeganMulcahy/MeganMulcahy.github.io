//table.h
#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct {
  ObjString* entryKey;
  Value entryValue;
} Entry;

typedef struct {
  int tableCount;
  int tableCapacity;
  Entry* tableEntries;
} Table;

void createTable(Table* table);

void deallocTable(Table* table);

bool findTableEntry(Table* table, ObjString* key, Value* value);

bool setTableEntry(Table* table, ObjString* key, Value value);

bool deleteTableEntry(Table* table, ObjString* key);

void mergeTables(Table* from, Table* to);

ObjString* findStringInTable(Table* table, const char* chars, int length, uint32_t hash);

void removeEmptyEntries(Table* table);

void markReferencesInTable(Table* table);

#endif
