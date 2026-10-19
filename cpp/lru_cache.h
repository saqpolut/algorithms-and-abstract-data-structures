#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "hash_table.h"

/*
Узел двусвязного списка. Хранит ключ, значение и указатели на предыдущий 
и следующий узлы. Конструктор инициализирует поля, 
что упрощает создание новых узлов.
*/
struct CacheNode {
    int key;
    int value;
    CacheNode* prev;
    CacheNode* next;

    CacheNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

struct LRUCache {
    int capacity; //максимальный размер
    int size; // текузий размер
    CacheNode* head;
    CacheNode* tail;
    HashTable table;

    explicit LRUCache(int cap);//запрещает неявное преобразование типов
    ~LRUCache();

    void set(int key, int value);
    int get(int key);

private:
    void removeNode(CacheNode* node);
    void addToHead(CacheNode* node);
    void moveToHead(CacheNode* node);
    CacheNode* removeTail();
};

#endif
