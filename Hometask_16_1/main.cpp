// Задача № 16. Поиск подстроки. (5 баллов)
//
// Найдите все вхождения шаблона в строку. Длина шаблона - p, длина строки - n.
// Время O(n + p), доп. память - O(p).
//
// № 16_1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).
//
// Формат входных данных:
// Шаблон, символ перевода строки, строка.
//
// Формат выходных данных:
// Позиции вхождения шаблона в строку.


// Время работы: O(p + n)
// Потребляемая память: O(p),
// где p - длина шаблона, n - длина строки во входных данных


#include <cassert>
#include <iostream>
#include <string>
#include <vector>


using std::cin;
using std::cout;
using std::string;
using std::vector;


// Время работы: O(pattern.length())
void ComputePrefixFunc(const string &pattern, vector<size_t> &pi) {
    pi[0] = 0;
    size_t j = 0;

    for (size_t i = 1; i < pattern.length(); i++) {
        while (pattern[i] != pattern[j] && j > 0) {
            j = pi[j - 1];
        }

        if (pattern[i] == pattern[j]) {
            j++;
        } else {
            assert(j == 0);
        }

        pi[i] = j;
    }
}


// Время работы: O(pattern.length() + text.length())
void Search(const string &pattern, const string &text, const vector<size_t> &pi,
            vector<size_t> &positions) {
    // Алгоритм работает так, как будто pattern и text соединены:
    // pattern + $ + text, однако в реальности это две различные переменные
    size_t j = 0;

    for (size_t i = 0; i < text.length(); i++) {
        assert(j <= pattern.length());
        // Ленивое вычисление: если j == pattern.length(), то следующее условие
        // не проверяется
        while ((j == pattern.length()) || (text[i] != pattern[j] && j > 0)) {
            j = pi[j - 1];
        }

        if (text[i] == pattern[j]) {
            j++;
        } else {
            assert(j == 0);
        }

        // Если префикс-функция для символа из text равняется длине pattern, то
        // то этот символ - конец вхождения pattern в text
        if (j == pattern.length()) {
            positions.push_back(i - pattern.length() + 1);
        }
    }
}


int main() {
    string pattern, text;
    cin >> pattern >> text;

    vector<size_t> pi(pattern.length());
    vector<size_t> positions;
    ComputePrefixFunc(pattern, pi);
    Search(pattern, text, pi, positions);

    for (size_t i : positions) {
        cout << i << " ";
    }

    return 0;
}
