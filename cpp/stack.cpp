#include "stack.h"
#include <iostream>
#include <fstream>
#include <string>

void initStack(Stack* stack) {
    stack->head = nullptr;
}

bool isStackEmpty(const Stack* stack) {
    return stack->head == nullptr;
}

void pushStack(Stack* stack, const std::string& value) {
    NodeS* new_node = new NodeS{value, stack->head};
    stack->head = new_node;
}

// Снимает вершину и ВОЗВРАЩАЕТ снятый элемент через outValue.
// false — стек пуст, элемент не тронут.
bool popStack(Stack* stack, std::string& outValue) {
    if (isStackEmpty(stack)) {
        return false;
    }
    NodeS* old_head = stack->head;
    outValue = old_head->data;        // сохраняем до delete

    stack->head = old_head->next;
    delete old_head;
    return true;
}

bool peekStack(const Stack* stack, std::string& value) {
    if (isStackEmpty(stack)) {
        return false;
    }
    value = stack->head->data;
    return true;
}

void printStack(const Stack* stack) {
    if (isStackEmpty(stack)) {
        std::cout << "пуст" << std::endl;
        return;
    }
    for (NodeS* current = stack->head; current != nullptr; current = current->next) {
        std::cout << current->data << " ";
    }
    std::cout << std::endl;
}

void destroyStack(Stack* stack) {
    std::string dummy;
    while (popStack(stack, dummy)) {}
}

static void saveStackHelper(NodeS* node, std::ofstream& out) {
    if (node == nullptr) return;
    saveStackHelper(node->next, out);
    out << node->data << "\n";
}

bool saveStackToFile(const Stack* stack, const char* filename) {
    std::ofstream out(filename);
    if (!out) return false;
    saveStackHelper(stack->head, out);
    return true;
}

bool loadStackFromFile(Stack* stack, const char* filename) {
    std::ifstream in(filename);
    if (!in) return false;

    destroyStack(stack);

    std::string val;
    while (in >> val) {
        pushStack(stack, val);
    }
    return true;
}