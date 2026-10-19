#ifndef GENEALOGY_H
#define GENEALOGY_H

#include <string>
#include <vector>
#include "hash_table.h"

/*
Структура, описывающая человека. 
Вместо указателей или строк для связей используются целочисленные индексы (ID) 
в массиве people.

    name: Имя человека.
    parent: Индекс родителя в массиве. -1 означает, что родителя нет (это родоначальник).
    descendants: Счетчик всех потомков.
    children: Список индексов детей этого человека.
*/
struct Person {
    std::string name;
    int parent = -1;
    int descendants = 0;
    std::vector<int> children;
};
//Объявление главной функции
void runGenealogy(HashMethod method, bool showStats);

#endif
