#include <iostream>
#include <string>
#include "stack.h"

using namespace std;

int main(){
    Stack stack;
    initStack(&stack);

    cout << "Введите операции бейсбольной игры (числа, C, D, +)." << endl;
    //cout << "Для завершения ввода нажмите Ctrl+D (Linux/Mac) или Ctrl+Z (Windows) и Enter." << endl;
    cout << "Пример ввода: 5 2 C D +" << endl;

    string operation;
    while(cin >> operation){
        if (operation == "C"){//удаление последнего записанного элемента
            string dummy;
            popStack(&stack, dummy);
        } 
        else if (operation == "D"){//удваивание последнего счёта
            string topValue;
            if (peekStack(&stack, topValue)){
                int score = stoi(topValue) * 2; //преобразование в число
                pushStack(&stack, to_string(score)); //преобразование в строку
            }
        } 
        else if (operation == "+"){
            string value1, value2;
            if (popStack(&stack, value1)){//временно удаляет верхний элемент
                if (peekStack(&stack, value2)) {// Смотрим на предпоследнее значение
                    int newScore = stoi(value1) + stoi(value2);
                    pushStack(&stack, value1);// Возвращаем последнее обратно
                    pushStack(&stack, to_string(newScore));// Добавляем новую сумму
                } 
                else {
                    // Страховка на случай некорректных данных (меньше 2 элементов)
                    pushStack(&stack, value1);
                }
            }
        } 
        else {
            // Целое число, записываем на вершину
            pushStack(&stack, operation);
        }
    }
    // Подсчет суммы всех оставшихся очков в стеке
    int totalSum = 0;
    string val;
    while (popStack(&stack, val)) {
        totalSum += stoi(val);
    }

    // Очистка памяти стека
    destroyStack(&stack);
    
    cout << "\nИтоговая сумма очков: " << totalSum << endl;

    return 0;
}