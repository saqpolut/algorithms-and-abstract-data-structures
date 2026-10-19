#ifndef SET_H
#define SET_H

#include <cstddef> //для size_t
#include <string>

//структура узла связного списка (для метода цепочек)
struct SetNode {
    std::string value;
    SetNode* next;
};
//основная структура множества
struct Set {
    SetNode** buckets; //массив указателей на головы связных списков
    std::size_t capacity; //размер массива
    std::size_t size; // текущее количество элементов в множестве
};

typedef bool (*SetVisitFunction)(const std::string& value, void* context);//обход элементов множества (определение типа указателя на функцию)

//создание
Set* set_create(std::size_t initial_capacity = 17);

//уничтожение
void set_destroy(Set* set);

//добавление, удаление, проверка наличия
bool set_add(Set* set, const std::string& value);
bool set_del(Set* set, const std::string& value);
bool set_at(const Set* set, const std::string& value);

//получение размера
std::size_t set_size(const Set* set);

//обход всех элементов
bool set_for_each(const Set* set, SetVisitFunction callback, void* context);


//доплнение для 3 задания
//объединение
Set* set_union_new(const Set* first, const Set* second);
//пересечение
Set* set_intersection_new(const Set* first, const Set* second);
//разность
Set* set_difference_new(const Set* first, const Set* second);

#endif
