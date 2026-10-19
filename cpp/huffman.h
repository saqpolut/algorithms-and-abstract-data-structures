#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

//хранит символ и частоту появления указатели
struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
};

//Связывает символ с его строковым бинарным кодом
struct HuffmanCode {
    char symbol;
    std::string code;
};


HuffmanNode* createLeaf(char symbol, int frequency);
HuffmanNode* createInternalNode(HuffmanNode* left, HuffmanNode* right);
HuffmanNode* buildHuffmanTree(const std::string& text);
void buildCodeTable(HuffmanNode* root, std::string currentCode, HuffmanCode* table, int& tableSize);
std::string encode(const std::string& text, const HuffmanCode* table, int tableSize);
std::string decode(const std::string& encoded, HuffmanNode* root);
void destroyTree(HuffmanNode* root);
void printCodeTable(const HuffmanCode* table, int tableSize);

#endif