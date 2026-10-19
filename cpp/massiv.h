#ifndef MASSIV_H
#define MASSIV_H

#include <cstddef>
#include <string>

struct Array
{
    std::string* data;
    size_t size;
    size_t capacity;
};

void create(Array& array);
void destroy(Array& array);
bool reserve(Array& array, size_t newCapacity);

bool addToEnd(Array& array, const std::string& value);
bool addAtIndex(Array& array, size_t index, const std::string& value);

bool getElement(const Array& array, size_t index, std::string& value);
size_t getLength(const Array& array);

void printArray(const Array& array);
bool readArray(Array& array);

bool removeAtIndex(Array& array, size_t index);
bool replaceAtIndex(Array& array, size_t index, const std::string& value);

bool saveArrayToFile(const Array& array, const char* filename);
bool loadArrayFromFile(Array& array, const char* filename);

#endif