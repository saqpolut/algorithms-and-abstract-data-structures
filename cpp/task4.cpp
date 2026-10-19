#include <iostream>
#include <string>
#include "massiv.h"


// Функция проверки соответствия строки шаблону
bool isMatch(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();

    // Создаем таблицу
    // dp[i][j] будет true, если первые i символов текста совпадают 
    // с первыми j символами паттерна
    bool** dp = new bool*[n + 1];
    for (int i = 0; i <= n; ++i) {
        dp[i] = new bool[m + 1];
    }

    // Инициализируем всю таблицу значением false
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            dp[i][j] = false;
        }
    }

    //Пустая строка совпадает с пустым паттерном
    dp[0][0] = true;

    //Обработка паттернов из одних звездочек для пустой строки
    // Например, паттерн "***" соответствует пустой строке
    for (int j = 1; j <= m; ++j) {
        if (pattern[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    //Заполнение таблицы
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            char pChar = pattern[j - 1];
            char tChar = text[i - 1];

            if (pChar == '?' || pChar == tChar) {
                // Символы совпадают или паттерн содержит '?'
                dp[i][j] = dp[i - 1][j - 1];
            } 
            else if (pChar == '*') {
                // '*' может заменять пустую строку
                // или один/несколько символов текста
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            }
        }
    }

    //Сохраняем результат из правой нижней ячейки
    bool result = dp[n][m];

    //Освобождаем память, выделенную под таблицу
    for (int i = 0; i <= n; ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}

int main() {

    Array inputData;
    create(inputData);

    std::cout << "=== Сопоставление с паттерном (Вариант 3) ===" << std::endl;
    std::cout << "Правила: '?' заменяет один символ, '*' заменяет любую последовательность." << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    std::string text, pattern;

    // Считываем текст и паттерн с клавиатуры
    std::cout << "\nВведите последовательность символов (текст): ";
    getline(std::cin, text);

    std::cout << "Введите шаблон (паттерн): ";
    getline(std::cin, pattern);

    // Добавляем данные в массив
    if (!addToEnd(inputData, text)) {
        std::cerr << "Ошибка: не удалось добавить текст в массив!" << std::endl;
        destroy(inputData);
        return 1;
    }

    if (!addToEnd(inputData, pattern)) {
        std::cerr << "Ошибка: не удалось добавить паттерн в массив!" << std::endl;
        destroy(inputData);
        return 1;
    }

    // Извлекаем данные из массива
    std::string storedText, storedPattern;
    getElement(inputData, 0, storedText);
    getElement(inputData, 1, storedPattern);

    // Выполнение алгоритма и вывод результата
    std::cout << "\n--- Результат ---" << std::endl;
    std::cout << "Текст:    \"" << storedText << "\"" << std::endl;
    std:: cout << "Паттерн:  \"" << storedPattern << "\"" << std::endl;

    if (isMatch(storedText, storedPattern)) {
        std::cout << "Вывод: Последовательность СООТВЕТСТВУЕТ шаблону." << std::endl;
    } else {
        std::cout << "Вывод: Последовательность НЕ СООТВЕТСТВУЕТ шаблону." << std::endl;
    }

    // Очистка памяти массива Array
    destroy(inputData);

    return 0;
}