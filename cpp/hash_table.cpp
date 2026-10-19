#include "hash_table.h"
#include <cstdint>

static size_t hash1(const std::string& key) {
    size_t hash = 5381;
    for (size_t i = 0; i < key.length(); ++i) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(key[i]);
    }
    return hash;
}
/*
Вторая хеш-функция, необходимая для двойного хеширования. 
Она должна отличаться от первой, чтобы шаг смещения 
был уникальным для каждого ключа и не кратным размеру таблицы.
*/
static size_t hash2(const std::string& key) {
    size_t hash = 0;
    for (size_t i = 0; i < key.length(); ++i) {
        hash = static_cast<unsigned char>(key[i]) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

//поиск простого числа
static bool isPrime(size_t number) {
    if (number < 2) return false;
    for (size_t i = 2; i * i <= number; ++i) {
        if (number % i == 0) return false;
    }
    return true;
}

static size_t nextPrime(size_t number) {
    while (!isPrime(number)) {
        ++number;
    }
    return number;
}

//вычисление шага
/*
Для линейного апробирования шаг всегда 1. Для двойного хеширования шаг равен 
1 + (hash2 % (capacity - 1)). 
Мы делаем % (capacity - 1), чтобы шаг никогда не был равен 0 
(иначе будет бесконечный цикл на одном месте) и всегда был меньше размера таблицы.
*/
static size_t getStep(const HashTable& table, const std::string& key) {
    if (table.method == LINEAR_PROBING) {
        return 1;
    }
    return 1 + (hash2(key) % (table.capacity - 1));
}

bool hashTableInit(HashTable& table, size_t expectedElements, HashMethod method) {
    table.capacity = nextPrime(expectedElements * 2 + 11);
    table.entries = new HashEntry[table.capacity];
    table.size = 0;
    table.method = method;
    table.probes = 0;
    table.collisions = 0;

    for (size_t i = 0; i < table.capacity; ++i) {
        table.entries[i].state = SLOT_EMPTY;
        table.entries[i].value = nullptr;
    }
    return true;
}

void hashTableDestroy(HashTable& table) {
    delete[] table.entries;
    table.entries = nullptr;
    table.capacity = 0;
    table.size = 0;
}

bool hashFind(HashTable& table, const std::string& key, void*& value) {
    if (table.capacity == 0) return false;

    size_t index = hash1(key) % table.capacity;
    size_t step = getStep(table, key);

    for (size_t checked = 0; checked < table.capacity; ++checked) {
        ++table.probes;
        HashEntry& entry = table.entries[index];

        if (entry.state == SLOT_EMPTY) {
            return false;
        }

        if (entry.state == SLOT_OCCUPIED && entry.key == key) {
            value = entry.value;
            return true;
        }

        ++table.collisions;
        index = (index + step) % table.capacity;
    }
    return false;
}

//вставка
bool hashInsert(HashTable& table, const std::string& key, void* value) {
    if (table.capacity == 0) return false;

    size_t index = hash1(key) % table.capacity;
    size_t step = getStep(table, key);
    size_t firstDeleted = table.capacity;

    for (size_t checked = 0; checked < table.capacity; ++checked) {
        ++table.probes;
        HashEntry& entry = table.entries[index];

        if (entry.state == SLOT_OCCUPIED && entry.key == key) {
            entry.value = value;
            return true;
        }

        if (entry.state == SLOT_DELETED && firstDeleted == table.capacity) {
            firstDeleted = index;
        }

        if (entry.state == SLOT_EMPTY) {
            size_t target = (firstDeleted != table.capacity) ? firstDeleted : index;
            table.entries[target].key = key;
            table.entries[target].value = value;
            table.entries[target].state = SLOT_OCCUPIED;
            ++table.size;
            return true;
        }

        ++table.collisions;
        index = (index + step) % table.capacity;
    }

    if (firstDeleted != table.capacity) {
        table.entries[firstDeleted].key = key;
        table.entries[firstDeleted].value = value;
        table.entries[firstDeleted].state = SLOT_OCCUPIED;
        ++table.size;
        return true;
    }

    return false;
}

//удаление. Мы меняем статус на SLOT_DELETED, а не на SLOT_EMPTY
bool hashErase(HashTable& table, const std::string& key) {
    if (table.capacity == 0) return false;

    size_t index = hash1(key) % table.capacity;
    size_t step = getStep(table, key);

    for (size_t checked = 0; checked < table.capacity; ++checked) {
        ++table.probes;
        HashEntry& entry = table.entries[index];

        if (entry.state == SLOT_EMPTY) {
            return false;
        }

        if (entry.state == SLOT_OCCUPIED && entry.key == key) {
            entry.state = SLOT_DELETED;
            entry.value = nullptr;
            --table.size;
            return true;
        }

        index = (index + step) % table.capacity;
    }
    return false;
}

// Обёртки для int
bool hashFindInt(HashTable& table, const std::string& key, int& value) {
    void* ptr = nullptr;
    if (hashFind(table, key, ptr)) {
        value = static_cast<int>(reinterpret_cast<intptr_t>(ptr));
        return true;
    }
    return false;
}

bool hashInsertInt(HashTable& table, const std::string& key, int value) {
    return hashInsert(table, key, reinterpret_cast<void*>(static_cast<intptr_t>(value)));
}
