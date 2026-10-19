#include "lru_cache.h"

/*Инициализация. Создаем фиктивные узлы head и tail и связываем их друг 
с другом. Это означает, что список изначально "пуст", но у него всегда 
есть начало и конец. Хеш-таблица инициализируется с размером, зависящим 
от емкости 
(минимум 8, чтобы избежать ошибок при cap = 0 или 1).
*/
LRUCache::LRUCache(int cap) : capacity(cap), size(0) {
    head = new CacheNode(-1, -1);
    tail = new CacheNode(-1, -1);
    head->next = tail;
    tail->prev = head;

    // Инициализируем собственную хеш-таблицу
    hashTableInit(table, static_cast<size_t>(cap > 0 ? cap : 8), DOUBLE_HASHING);
}

LRUCache::~LRUCache() {
    CacheNode* curr = head;
    while (curr != nullptr) {
        CacheNode* next = curr->next;
        delete curr;
        curr = next;
    }
    hashTableDestroy(table);
}

/*
Вырезает узел из списка, перенаправляя указатели его соседей друг 
на друга. Узел при этом не удаляется из памяти (delete не вызывается), 
он просто отсоединяется.
*/
void LRUCache::removeNode(CacheNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/*
Вставляет узел сразу после фиктивного head. Благодаря фиктивным узлам здесь не нужно проверять, 
пуст ли список или нет
*/
void LRUCache::addToHead(CacheNode* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}
/*
Комбинация двух предыдущих функций. Используется при GET или 
обновлении SET, чтобы пометить 
элемент как "только что использованный"
*/
void LRUCache::moveToHead(CacheNode* node) {
    removeNode(node);
    addToHead(node);
}
/*
Находит узел прямо перед фиктивным tail, вырезает его из списка и 
возвращает указатель на него для последующего удаления.
*/
CacheNode* LRUCache::removeTail() {
    CacheNode* node = tail->prev;
    removeNode(node);
    return node;
}

/*
Преобразуем int ключ в строку (т.к. наша хеш-таблица работает со string). 
Ищем в хеш-таблице. Если нет — возвращаем -1.
Если есть, извлекаем указатель на узел, перемещаем его в начало списка 
обновляем его "свежесть" и возвращаем значение.
*/
int LRUCache::get(int key) {
    std::string keyStr = std::to_string(key);
    void* ptr = nullptr;

    if (!hashFind(table, keyStr, ptr)) {
        return -1;
    }

    CacheNode* node = static_cast<CacheNode*>(ptr);
    moveToHead(node);
    return node->value;
}
/*
Если емкость 0 или меньше, ничего не делаем. Проверяем, есть ли ключ 
уже в кэше. Если да, просто обновляем значение и 
перемещаем узел в начало (он стал самым "свежим").
*/
void LRUCache::set(int key, int value) {
    if (capacity <= 0) return;

    std::string keyStr = std::to_string(key);
    void* ptr = nullptr;

    if (hashFind(table, keyStr, ptr)) {
        CacheNode* node = static_cast<CacheNode*>(ptr);
        node->value = value;
        moveToHead(node);
        return;
    }
    /*
    Если кэш полон, освобождаем место
    Берем самый старый узел (removeTail), удаляем его ключ из хеш-таблицы, 
    освобождаем память (delete) и уменьшаем счетчик размера
    */
    if (size >= capacity) {
        CacheNode* oldest = removeTail();
        hashErase(table, std::to_string(oldest->key)); // удаляем из хеш-таблицы
        delete oldest;
        --size;
    }

    CacheNode* newNode = new CacheNode(key, value);
    addToHead(newNode);
    hashInsert(table, keyStr, newNode); // сохраняем узел в хеш-таблицу
    ++size;
}
