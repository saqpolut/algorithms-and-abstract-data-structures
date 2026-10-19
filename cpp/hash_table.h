#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstddef>
#include <string>

enum HashMethod {
    LINEAR_PROBING,
    DOUBLE_HASHING
};

enum SlotState {
    SLOT_EMPTY,
    SLOT_OCCUPIED, //содержит актуальные данные
    SLOT_DELETED
};

struct HashEntry {
    std::string key;
    void* value;
    SlotState state;
};

struct HashTable {
    HashEntry* entries;
    size_t capacity;
    size_t size;
    HashMethod method;
    size_t probes; // Счетчик для эмпирического анализа (сколько ячеек проверили)
    size_t collisions;// Счетчик для эмпирического анализа (сколько раз был сдвиг)
};

// Базовые операции
bool hashTableInit(HashTable& table, size_t expectedElements, HashMethod method);
void hashTableDestroy(HashTable& table);
bool hashFind(HashTable& table, const std::string& key, void*& value);
bool hashInsert(HashTable& table, const std::string& key, void* value);
bool hashErase(HashTable& table, const std::string& key);

// Удобные обёртки для целых чисел
bool hashFindInt(HashTable& table, const std::string& key, int& value);
bool hashInsertInt(HashTable& table, const std::string& key, int value);

#endif
