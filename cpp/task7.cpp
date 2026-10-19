#include "lru_cache.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cerr << "=== Задание 7: LRU-кэш ===\n"
              << "Введите ёмкость кэша (cap) и количество запросов (Q):\n> ";

    int cap, q;
    if (!(std::cin >> cap >> q) || cap < 0 || q < 0) {
        std::cerr << "Ошибка: cap и Q должны быть неотрицательными числами!\n";
        return 1;
    }

    LRUCache cache(cap);

    std::cerr << "Введите " << q << " запросов (SET <key> <val> или GET <key>):\n";

    std::vector<int> results;
    //bool first = true;

    for (int i = 0; i < q; ++i) {
        std::cerr << "Запрос [" << (i + 1) << "/" << q << "]: ";

        std::string op;
        if (!(std::cin >> op)) {
            std::cerr << "\nОшибка чтения команды.\n";
            break;
        }

        if (op == "SET") {
            int k, v;
            if (!(std::cin >> k >> v)) {
                std::cerr << "Ошибка: для SET нужны два числа (ключ и значение).\n";
                break;
            }
            cache.set(k, v);
        } else if (op == "GET") {
            int k;
            if (!(std::cin >> k)) {
                std::cerr << "Ошибка: для GET нужно число (ключ).\n";
                break;
            }
            //if (!first) std::cout << " ";
            //std::cout << cache.get(k);
            //first = false;
            results.push_back(cache.get(k));
        } else {
            std::cerr << "Неизвестная операция: " << op 
                      << " (допустимы только SET и GET)\n";
        }
    }
    std::cerr << "\nВывод:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        if (i > 0) std::cout << " ";
        std::cout << results[i];
    }
    std::cout << "\n";
    return 0;
}
