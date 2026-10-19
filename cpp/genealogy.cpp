#include "genealogy.h"
#include <iostream>
#include <algorithm>

//Принимает массив людей, ID текущего человека и глубину вложенности
static void printTree(const std::vector<Person>& people, int id, int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    //выводим имя и количество потомков текущего узла
    std::cout << people[id].name << " (" << people[id].descendants << " потомков)" << std::endl;

    for (int childId : people[id].children) {
        printTree(people, childId, depth + 1);
    }
}

void runGenealogy(HashMethod method, bool showStats) {
    std::cerr << "=== Задание 6: Родословная (число потомков) ===" << std::endl
              << "Введите количество людей N: ";

    int n;
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Ошибка: число N должно быть > 0" << std::endl;
        return;
    }

    std::cerr << "Введите " << (n - 1) << " пар вида: <имя_потомка> <имя_родителя>" << std::endl;

    HashTable nameTable;
    hashTableInit(nameTable, static_cast<size_t>(n), method);

    std::vector<Person> people;
    people.reserve(n);


    auto getOrAddPerson = [&](const std::string& name) -> int {
        int id = -1;
        if (hashFindInt(nameTable, name, id)) {
            return id;
        }
        id = static_cast<int>(people.size());
        Person p;
        p.name = name;
        people.push_back(p);
        hashInsertInt(nameTable, name, id);
        return id;
    };

    //чтение пар и построение связей
    for (int i = 0; i < n - 1; ++i) {
        std::cerr << "Пара [" << (i + 1) << "/" << (n - 1) << "]: ";

        std::string childName, parentName;
        if (!(std::cin >> childName >> parentName)) {
            std::cerr << "\nОшибка чтения пары потомка и родителя." << std::endl;
            hashTableDestroy(nameTable);
            return;
        }

        //получаем ID ребенка и родителя. Добавляем связи
        int child = getOrAddPerson(childName);
        int parent = getOrAddPerson(parentName);

        people[child].parent = parent;
        people[parent].children.push_back(child);
    }

    int root = -1;
    for (int i = 0; i < n; ++i) {
        if (people[i].parent == -1) {
            root = i;
            break;
        }
    }

    // Подсчет потомков подъемом от каждого узла к корню
    for (int i = 0; i < n; ++i) {
        int ancestor = people[i].parent;
        while (ancestor != -1) {
            people[ancestor].descendants++;
            ancestor = people[ancestor].parent;
        }
    }

    // Сортировка для вывода
    std::vector<int> sortedIds(n);
    for (int i = 0; i < n; ++i) sortedIds[i] = i;

    std::sort(sortedIds.begin(), sortedIds.end(), [&](int a, int b) {
        return people[a].name < people[b].name;
    });

    std::cerr << "\n--- Список элементов с числом потомков ---\n";
    for (int id : sortedIds) {
        std::cout << people[id].name << " " << people[id].descendants << std::endl;
    }

    // Сортировка детей для аккуратного вывода дерева
    for (Person& p : people) {
        std::sort(p.children.begin(), p.children.end(), [&](int a, int b) {
            return people[a].name < people[b].name;
        });
    }

    std::cout << std::endl << "Дерево:" << std::endl;
    if (root != -1) {
        printTree(people, root, 0);
    }

    if (showStats) {
        std::cerr << std::endl << "[Статистика хеш-таблицы]" << std::endl
                  << "Проверок ячеек: " << nameTable.probes << std::endl
                  << "Коллизий: " << nameTable.collisions << std::endl;
    }

    hashTableDestroy(nameTable);
}
