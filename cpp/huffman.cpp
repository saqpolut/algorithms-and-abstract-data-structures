#include "huffman.h"
#include <iostream>
#include <cstddef>

namespace {

    //если узел существует и у него нет ни левого ни правого потомков - true
bool isLeaf(const HuffmanNode* node) {
    return node != nullptr && node->left == nullptr && node->right == nullptr;
}

//рекурсивная функция обхода дерева
//если узел пустой, то просто выходим
void buildCodeTableRecursive(
    HuffmanNode* node,
    const std::string& currentCode,
    HuffmanCode* table,
    int& tableSize
) {
    if (node == nullptr) {
        return;
    }

    if (isLeaf(node)) {
        table[tableSize].symbol = node->symbol;
        // Если в строке всего 1 уникальный символ, даём ему код "0"
        table[tableSize].code = currentCode.empty() ? "0" : currentCode;
        ++tableSize;
        return;
    }

    buildCodeTableRecursive(node->left, currentCode + "0", table, tableSize);
    buildCodeTableRecursive(node->right, currentCode + "1", table, tableSize);
}

}

//создание узла
HuffmanNode* createLeaf(char symbol, int frequency) {
    HuffmanNode* node = new HuffmanNode;
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

//создаёт внутренний узел. Его вес равен сумме весов детей
HuffmanNode* createInternalNode(HuffmanNode* left, HuffmanNode* right) {
    HuffmanNode* node = new HuffmanNode;
    node->symbol = '\0';
    node->frequency = left->frequency + right->frequency;
    node->left = left;
    node->right = right;
    return node;
}

//начало построения дерева. если строка пустая, возвращаем пустой указатель
HuffmanNode* buildHuffmanTree(const std::string& text) {
    if (text.empty()) {
        return nullptr;
    }

    // Подсчёт частот всех байтов (ASCII)
    int frequencies[256] = {};
    for (size_t i = 0; i < text.length(); ++i) {
        unsigned char symbol = static_cast<unsigned char>(text[i]);
        ++frequencies[symbol];
    }

    // Создаём массив листьев для всех встретившихся символов
    HuffmanNode* nodes[512];
    int nodeCount = 0;

    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            nodes[nodeCount] = createLeaf(static_cast<char>(i), frequencies[i]);
            ++nodeCount;
        }
    }

    // Пошагово объединяем два узла с наименьшими весами
    while (nodeCount > 1) {
        //ищем индекс с наименьшей частотой
        int firstIndex = 0;
        for (int i = 1; i < nodeCount; ++i) {
            if (nodes[i]->frequency < nodes[firstIndex]->frequency) {
                firstIndex = i;
            }
        }
        //ищем индекс узла со второй наименьшей частотой
        int secondIndex = -1;
        for (int i = 0; i < nodeCount; ++i) {
            if (i == firstIndex) continue;
            if (secondIndex == -1 || nodes[i]->frequency < nodes[secondIndex]->frequency) {
                secondIndex = i;
            }
        }
        //сохраняем указатели на эти узлы
        HuffmanNode* first = nodes[firstIndex];
        HuffmanNode* second = nodes[secondIndex];

        // Удаляем выбранные узлы из списка свободных
        //сдвиг всех последующих элементов на одну позицию влево, перезаписывая удаляемый элемент
        if (firstIndex > secondIndex) {
            int temp = firstIndex;
            firstIndex = secondIndex;
            secondIndex = temp;
        }

        for (int i = secondIndex; i < nodeCount - 1; ++i) {
            nodes[i] = nodes[i + 1];
        }
        --nodeCount;

        for (int i = firstIndex; i < nodeCount - 1; ++i) {
            nodes[i] = nodes[i + 1];
        }
        --nodeCount;

        //левая дуга — 0, правая — 1
        nodes[nodeCount] = createInternalNode(first, second);
        ++nodeCount;
    }

    return nodes[0];
}

void buildCodeTable(
    HuffmanNode* root,
    std::string currentCode,
    HuffmanCode* table,
    int& tableSize
) {
    tableSize = 0;
    buildCodeTableRecursive(root, currentCode, table, tableSize);
}

//функция кодирования
// Для каждого символа исходной строки ищем его в таблице. 
//Если нашли, добавляем его код к result. 
//Если вдруг символ не найден (ошибка), возвращаем пустую строку
std::string encode(
    const std::string& text,
    const HuffmanCode* table,
    int tableSize
) {
    std::string result = "";
    for (size_t i = 0; i < text.length(); ++i) {
        bool found = false;
        for (int j = 0; j < tableSize; ++j) {
            if (table[j].symbol == text[i]) {
                result += table[j].code;
                found = true;
                break;
            }
        }
        if (!found) {
            return "";
        }
    }
    return result;
}

std::string decode(const std::string& encoded, HuffmanNode* root) {
    if (root == nullptr || encoded.empty()) {
        return "";
    }

    // Если всё дерево — один символ
    if (isLeaf(root)) {
        std::string result = "";
        for (size_t i = 0; i < encoded.length(); ++i) {
            if (encoded[i] != '0') return "";
            result += root->symbol;
        }
        return result;
    }
    /*Идем по дереву: '0' — влево, '1' — вправо. 
    Если встретили недопустимый символ или тупик (nullptr), возвращаем ошибку. 
    Если достигли листа, добавляем его символ в result и возвращаемся к корню 
    (current = root) для декодирования следующего символа.
    */
    std::string result = "";
    HuffmanNode* current = root;

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '0') {
            current = current->left;
        } else if (encoded[i] == '1') {
            current = current->right;
        } else {
            return ""; // Недопустимый символ в коде
        }

        if (current == nullptr) {
            return "";
        }

        if (isLeaf(current)) {
            result += current->symbol;
            current = root; // Снова спускаемся от корня
        }
    }

    if (current != root) {
        return ""; // Неполная битовая цепочкаесли после цикла мы не вернулись к корню,
        // значит, код оборвался на полпути (неполный). 
        //Возвращаем результат
    }

    return result;
}

void destroyTree(HuffmanNode* root) {
    if (root == nullptr) {
        return;
    }
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

void printCodeTable(const HuffmanCode* table, int tableSize) {
    std::cout << std::endl << "Таблица кодов:" << std::endl;
    for (int i = 0; i < tableSize; ++i) {
        std::cout << "'" << table[i].symbol << "' : " << table[i].code << std::endl;
    }
}
