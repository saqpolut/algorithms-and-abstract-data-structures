//предотвращение повторного включения этого файла в один и тот же исходный код
#ifndef STACK_H
#define STACK_H

#include <string>

// Структура узла стека
struct NodeS {
    std::string data;
    NodeS* next;
};

// Структура самого стека
struct Stack {
    NodeS* head;
};

void initStack(Stack* stack);
bool isStackEmpty(const Stack* stack);

void pushStack(Stack* stack, const std::string& value);
bool popStack(Stack* stack, std::string& outValue);       
bool peekStack(const Stack* stack, std::string& value);       // вершина без удаления

void printStack(const Stack* stack);
void destroyStack(Stack* stack);

bool saveStackToFile(const Stack* stack, const char* filename);
bool loadStackFromFile(Stack* stack, const char* filename);

#endif // STACK_H