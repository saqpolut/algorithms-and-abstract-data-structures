#include <iostream>
#include <string>

#include "huffman.h"


int main() {

    std::cout << "=== Алгоритм Хаффмана ===" << std::endl;
    std::cout << "Введите исходную строку для построения дерева: ";

    std::string text;
    getline(std::cin, text);

    if (text.empty()) {
        std::cerr << "Ошибка: исходная строка не должна быть пустой." << std::endl;
        return 1;
    }

    // Дерево строится по частотам символов исходной строки.
    HuffmanNode* root = buildHuffmanTree(text);

    if (root == nullptr) {
        std::cerr << "Ошибка: не удалось построить дерево." << std::endl;
        return 1;
    }

    HuffmanCode table[256];
    int tableSize = 0;
    buildCodeTable(root, "", table, tableSize);

    printCodeTable(table, tableSize);

    std::cout << std::endl << "Исходная строка: " << text << std::endl;
    std::cout << "Её код: " << encode(text, table, tableSize) << std::endl;

    std::string choice;

    while (true) {
        std::cout << std::endl << "Выберите действие:" << std::endl;
        std::cout << "1 — показать код исходной строки" << std::endl;
        std::cout << "2 — декодировать введённый двоичный код" << std::endl;
        std::cout << "0 — выход" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!getline(std::cin, choice)) {
            break;
        }

        if (choice == "0") {
            break;
        }

        if (choice == "1") {
            std::cout << "Закодированная строка: "
                 << encode(text, table, tableSize) << std::endl;
        } else if (choice == "2") {
            std::cout << "Введите двоичный код: ";

            std::string encoded;
            if (!getline(std::cin, encoded)) {
                break;
            }

            if (encoded.empty()) {
                std::cout << "Ошибка: код не должен быть пустым." << std::endl;
                continue;
            }

            std::string decoded = decode(encoded, root);

            if (decoded.empty()) {
                std::cout << "Ошибка: код некорректен или неполон." << std::endl;
            } else {
                std::cout << "Расшифрованная строка: " << decoded << std::endl;
            }
        } else {
            std::cout << "Введите 0, 1 или 2." << std::endl;
        }
    }

    destroyTree(root);
    return 0;
}
