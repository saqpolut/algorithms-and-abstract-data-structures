//создание, добавление элемента (по индексу и в конец массива), получение элемента по индексу
//удаление элемента по индексу, замена элемента по индексу, длина массива, чтение
#include "massiv.h"
#include <iostream>
#include <fstream>
#include <new>
#include <string>

using namespace std;

//создваётся пустой
void create(Array& array){
    array.data = nullptr;
    array.size = 0;
    array.capacity = 0;
}

//освобождение памяти
void destroy(Array& array){
    delete[] array.data;
    array.data = nullptr;
    array.size = 0;
    array.capacity = 0;
}

//заранее расширяет блок (копирование)
bool reserve(Array& array, size_t newCapacity){
    if (newCapacity <= array.capacity){
        return true;
    }

    string* newData = new (nothrow) string[newCapacity]; //создаётся новый блок
    if (newData == nullptr){
        return false;
    }

    for(size_t i=0; i < array.size; ++i){ //копируем размер старых элементов
        newData[i] = array.data[i];
    }

    delete[] array.data;
    array.data = newData;
    array.capacity = newCapacity;
    return true;
}

//добавление в конец
bool addToEnd(Array& array, const string& value){
    if (array.size == array.capacity){ // проверка на наличие места в массиве
        size_t newCapacity = (array.capacity == 0) ? 1 : array.capacity * 2; //
        if (!reserve(array, newCapacity)){
            return false;
        }
    }

    array.data[array.size] = value; //пишем за последним в конец
    ++array.size;
    return true;
}

//вставка по индексу
bool addAtIndex(Array& array, size_t index, const string& value){
    if (index > array.size){
        return false;
    }

    if (array.size == array.capacity){
        size_t newCapacity = (array.capacity == 0) ? 1 : array.capacity * 2;
        if (!reserve(array, newCapacity)){
            return false;
        }
    }

    for (size_t i = array.size; i > index; --i){
        array.data[i] = array.data[i-1]; // сдвиг идёт с конца
    }

    array.data[index] = value;
    ++array.size;
    return true;
}

//получение элемента (доступ к нему)
bool getElement(const Array& array, size_t index, string& value){
    if (index >= array.size){
        return false;
    }

    value = array.data[index];
    return true;
}

// получение длины массива
size_t getLength(const Array& array){
    return array.size;
}

//удаление по индексу со сдвигом влево
bool removeAtIndex(Array& array, size_t index){
    if (index >= array.size){
        return false;
    }

    for (size_t i = index; i + 1 < array.size; ++i){
        array.data[i] = array.data[i + 1]; //идём с начала чтобы не затереть ещё не сдвинутые
    }

    --array.size;
    return true;
}

//изменение элемента по индексу
bool replaceAtIndex(Array& array, size_t index, const string& value){
    if (index >= array.size){
        return false;
    }

    array.data[index] = value;
    return true;
}

//чтение массива (ввод с клавиатуры)
bool readArray(Array& array) {
    size_t count;

    cout << "Введите количество элементов: ";
    if (!(cin >> count)) {
        return false;
    }

    string* newData = nullptr;
    if (count > 0){
        newData = new string[count];
    }
    if (count > 0 && newData == nullptr) {
        return false;
    }

    cout << "Введите элементы массива: ";
    for (size_t i = 0; i < count; ++i) {
        if (!(cin >> newData[i])) {
            delete[] newData;
            return false;
        }
    }

    delete[] array.data;
    array.data = newData;
    array.size = count;
    array.capacity = count;
    return true;
}


//печатает через пробел элементы массива
void printArray(const Array& array) {
    if (array.size == 0){
        cout << "Массив пуст " << endl;
        return;
    }
    for (size_t i = 0; i < array.size; ++i) {
        cout << array.data[i] << ' ';
    }
    cout << endl;
}

//сохранение массива в файл
bool saveArrayToFile(const Array& array, const char* filename) {
    ofstream out(filename);
    if (!out) {
        return false;
    }

    out << array.size << endl;
    for (size_t i = 0; i < array.size; ++i) {
        out << array.data[i] << " ";
    }
    out << endl;

    return true;
}

//из файла
bool loadArrayFromFile(Array& array, const char* filename) {
    ifstream in(filename);
    if (!in) {
        return false;
    }

    destroy(array);  // Очищаем старый массив

    size_t count;
    if (!(in >> count)) {
        return false;
    }

    for (size_t i = 0; i < count; ++i) {
        string val;
        if (!(in >> val)) {
            return false;
        }
        addToEnd(array, val);
    }

    return true;
}