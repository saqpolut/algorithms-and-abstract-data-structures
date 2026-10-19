#include "set.h"

#include <cstddef>
#include <string>

// Вспомогательные функции и структуры используются только в этом файле.
namespace {

std::size_t hash_string(const std::string& value, std::size_t capacity) {
    std::size_t hash = 5381;

    for (unsigned char ch : value) {
        hash = hash * 33 + ch;
    }

    return hash % capacity;
}

//исменение размера. Создаёт новый массив большего размера и инициализирует его
void rehash(Set* set, std::size_t new_capacity) {
    SetNode** new_buckets = new SetNode*[new_capacity];

    for (std::size_t i = 0; i < new_capacity; ++i) {
        new_buckets[i] = nullptr;
    }

    // Переносим узлы в новые корзины, не создавая копии элементов.
    for (std::size_t i = 0; i < set->capacity; ++i) {
        SetNode* current = set->buckets[i];

        while (current != nullptr) { //исменение указателей
            SetNode* next = current->next;
            std::size_t index = hash_string(current->value, new_capacity);

            current->next = new_buckets[index];
            new_buckets[index] = current;

            current = next;
        }
    }

    //освобождает старый массив и обновляет указатели и ёмкость
    delete[] set->buckets;
    set->buckets = new_buckets;
    set->capacity = new_capacity;
}

//объединение
struct AddContext {
    Set* result;
};

//добавляет элемент в новое множество
bool add_to_result(const std::string& value, void* context) {//указатель
    AddContext* data = static_cast<AddContext*>(context); //оператор явного приведения типов

    // Уже имеющийся элемент
    set_add(data->result, value);
    return true;
}
//фильтр для пересечения и разности
//проверяет, есть ли элемент в другом множестве. Если элемент совпадает с флагом, элемнт добавляется в результат
struct FilterContext {
    const Set* other;
    Set* result;
    bool keep_if_present;//true - пересечение false - разность
};

bool filter_into_result(const std::string& value, void* context) {
    FilterContext* data = static_cast<FilterContext*>(context);
    bool is_present = set_at(data->other, value);

    if (is_present == data->keep_if_present) {
        set_add(data->result, value);
    }

    return true;
}

}

//создание множества
Set* set_create(std::size_t initial_capacity) {
    if (initial_capacity < 17) {
        initial_capacity = 17;
    }
//выделяет память под структуру и массив указателей
    Set* set = new Set;
    set->capacity = initial_capacity;
    set->size = 0;
    set->buckets = new SetNode*[set->capacity];

    for (std::size_t i = 0; i < set->capacity; ++i) {
        set->buckets[i] = nullptr;
    }

    return set;
}

//очистка памяти
//проходит по каждой корзине, удаляет все узлы в цепочке, затем удаляет ам массив и сруктуру
void set_destroy(Set* set) {
    if (set == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < set->capacity; ++i) {
        SetNode* current = set->buckets[i];

        while (current != nullptr) {
            SetNode* next = current->next;
            delete current;
            current = next;
        }
    }

    delete[] set->buckets;
    delete set;
}

//проверка наличия элемента
bool set_at(const Set* set, const std::string& value) {
    if (set == nullptr || set->capacity == 0) {
        return false;
    }

    std::size_t index = hash_string(value, set->capacity);
    SetNode* current = set->buckets[index];

    while (current != nullptr) {
        if (current->value == value) {
            return true;
        }

        current = current->next;
    }

    return false;
}

//если элемент уже есть, добавление не производится
bool set_add(Set* set, const std::string& value) {
    if (set == nullptr) {
        return false;
    }

    // Множество не должно содержать повторяющиеся элементы.
    if (set_at(set, value)) {
        return false;
    }

    // Увеличиваем таблицу, если занято больше 75% корзин.
    if ((set->size + 1) * 4 > set->capacity * 3) {
        rehash(set, set->capacity * 2 + 1);
    }

//создаётся новый узел и вставляется в начало
    std::size_t index = hash_string(value, set->capacity);
    SetNode* node = new SetNode{value, set->buckets[index]};

    set->buckets[index] = node;
    ++set->size;

    return true;
}

//удаление элемента
bool set_del(Set* set, const std::string& value) {
    if (set == nullptr || set->capacity == 0) {
        return false;
    }

    std::size_t index = hash_string(value, set->capacity);
    SetNode* current = set->buckets[index];
    SetNode* previous = nullptr;

    while (current != nullptr) {
        if (current->value == value) {
            if (previous == nullptr) {
                set->buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }

            delete current;
            --set->size;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

//получение количества элементов
std::size_t set_size(const Set* set) {
    if (set == nullptr) {
        return 0;
    }

    return set->size;
}

bool set_for_each(
    const Set* set,
    SetVisitFunction callback,
    void* context
) {
    if (set == nullptr || callback == nullptr) {
        return false;
    }

    for (std::size_t i = 0; i < set->capacity; ++i) {
        SetNode* current = set->buckets[i];

        while (current != nullptr) {
            if (!callback(current->value, context)) {
                return false;
            }

            current = current->next;
        }
    }

    return true;
}

//объединение. Создает новое множество и последовательно добавляет в него все элементы первого и второго
Set* set_union_new(const Set* first, const Set* second) {
    if (first == nullptr || second == nullptr) {
        return nullptr;
    }

    Set* result = set_create();
    AddContext context{result};

    set_for_each(first, add_to_result, &context);
    set_for_each(second, add_to_result, &context);

    return result;
}

//пересечение. Находит множество с меньшим размером и перебирает его элементы, проверяя их наличие в большом множестве
Set* set_intersection_new(const Set* first, const Set* second) {
    if (first == nullptr || second == nullptr) {
        return nullptr;
    }

    Set* result = set_create();

    // Перебираем меньшее множество, проверяя элементы в большем.
    const Set* smaller = first;
    const Set* larger = second;

    if (set_size(first) > set_size(second)) {
        smaller = second;
        larger = first;
    }

    FilterContext context{larger, result, true};
    set_for_each(smaller, filter_into_result, &context);

    return result;
}

//разность
Set* set_difference_new(const Set* first, const Set* second) {
    if (first == nullptr || second == nullptr) {
        return nullptr;
    }

    Set* result = set_create();

    // Разность first \ second:
    // добавляем элементы first, которых нет в second.
    FilterContext context{second, result, false};
    set_for_each(first, filter_into_result, &context);

    return result;
}
