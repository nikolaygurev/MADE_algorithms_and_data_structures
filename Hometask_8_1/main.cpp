// Задача 8. Хэш таблица. (10 баллов)
//
// Реализуйте структуру данных типа "множество строк" на основе динамической
// хэш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
// строчных латинских букв.
// Хэш-функция строки должна быть реализована с помощью вычисления значения
// многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехэширование выполняйте
// при добавлении элементов в случае, когда коэффициент заполнения таблицы
// достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество,
// удаления строки из множества и проверки принадлежности данной строки
// множеству.
//
// № 8_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая
// проба: g(k, i) = (g(k, i - 1) + i) mod m, где m - степень двойки.
//
// Формат входных данных:
// Каждая строка входных данных задает одну операцию над множеством. Запись
// операции состоит из типа операции и следующей за ним через пробел строки, над
// которой проводится операция.
//
// Тип операции - один из трех символов:
// '+' - добавление строки в множество;
// '-' - удаление строки из множества;
// '?' - проверка принадлежности строки множеству.
//
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
// этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
// присутствует в этом множестве.
//
// Формат выходных данных:
// Программа должна вывести для каждой операции одну из двух строк "OK" или
// "FAIL".
//
// Для операции '?': "OK", если элемент присутствует во множестве. "FAIL" иначе.
// Для операции '+': "FAIL", если добавляемый элемент уже присутствует во
// множестве и потому не может быть добавлен. "OK" иначе.
// Для операции '-': "OK", если элемент присутствовал во множестве и успешно
// удален. "FAIL" иначе.


// Время работы: O(n)
// Потребляемая память: O(n)


#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


using std::cin;
using std::cout;
using std::endl;
using std::move;
using std::overflow_error;
using std::string;
using std::vector;


class HashTable {
private:
    // Вектор флажков, обозначающих удаленные элементы
    vector<bool> deleted;

    vector<string> table;
    const int random_value;
    double current_size = 0;

    size_t GetBaseHash(const string &key) const;

    size_t GetNewHash(size_t current_hash, size_t current_iter) const;

    void Rehash();

public:
    explicit HashTable(size_t initial_size, int random_value_);

    // Констурктор копирования
    HashTable(const HashTable &) = delete;

    // Конструктор перемещения
    HashTable(HashTable &&) = delete;

    // Оператор присваивания копированием
    HashTable &operator=(const HashTable &) = delete;

    // Оператор присваивания перемещением
    HashTable &operator=(HashTable &&) = delete;

    bool Has(const string &key) const;

    bool Add(const string &key);

    bool Remove(const string &key);
};


HashTable::HashTable(size_t initial_size, int random_value_)
        : table(initial_size, ""),
          deleted(initial_size, false),
          random_value(random_value_) {}


// Среднее время работы: O(1) (для длинных ключей: O(key.size()))
size_t HashTable::GetBaseHash(const string &key) const {
    assert(!key.empty());
    size_t hash = 0;

    for (char ch : key) {
        hash = (hash * random_value + ch) % table.size();
    }

    return hash;
}


// Время работы: O(1)
size_t HashTable::GetNewHash(size_t current_hash, size_t current_iter) const {
    assert(current_hash >= 0);

    return (current_hash + (current_iter + 1)) % table.size();
}


// Время работы: O(n)
void HashTable::Rehash() {
    // Сохраняем старые значения векторов
    auto old_table = move(table);
    auto old_deleted = move(deleted);

    // Создаем новые вектора в 2 раза больше
    vector<string> new_table(old_table.size() * 2, "");
    vector<bool> new_deleted(old_table.size() * 2, false);
    table = move(new_table);
    deleted = move(new_deleted);

    // Обнуляем количество хранимых элементов
    current_size = 0;

    // Переносим ключи в новый вектор
    for (size_t i = 0; i < old_table.size(); i++) {
        if ((!old_table[i].empty()) && (!old_deleted[i])) {
            Add(old_table[i]);
        }
    }
}


// Среднее время работы: O(1)
bool HashTable::Has(const string &key) const {
    size_t hash = GetBaseHash(key);

    for (size_t i = 0; i < table.size(); i++) {
        // Ячейка была и есть пуста
        if (table[hash].empty()) {
            assert(!deleted[hash]);
            return false;
        }

        // Сейчас в этой ячейке текущий ключ
        if ((table[hash] == key) && (!deleted[hash])) {
            return true;
        }

        // Раньше в этой ячейке был текущий ключ
        if ((table[hash] == key) && (deleted[hash])) {
            return false;
        }
        hash = GetNewHash(hash, i);
    }

    throw overflow_error("");
}


// Среднее время работы: O(1)
bool HashTable::Add(const string &key) {
    size_t hash = GetBaseHash(key);

    // Позиция, в которую будет вставлен текущий ключ, если он еще отсутствует
    size_t pos_for_insert = -1;
    // Флажок, принимающий значение true, когда найдена pos_for_insert
    bool is_pos_found = false;

    for (size_t i = 0; i < table.size(); i++) {
        // Ячейка была и есть пуста
        if (table[hash].empty()) {
            assert(!deleted[hash]);

            if (!is_pos_found) {
                pos_for_insert = hash;
            }

            table[pos_for_insert] = key;
            deleted[pos_for_insert] = false;
            current_size++;

            if (current_size / table.size() >= 0.75) {
                Rehash();
            }
            return true;
        }

        // Сейчас в этой ячейке текущий ключ
        if ((table[hash] == key) && (!deleted[hash])) {
            return false;
        }

        // Раньше в этой ячейке был какой-то ключ
        if (deleted[hash]) {
            if (!is_pos_found) {
                pos_for_insert = hash;
                is_pos_found = true;
            }
        }
        hash = GetNewHash(hash, i);
    }

    throw overflow_error("");
}


// Среднее время работы: O(1)
bool HashTable::Remove(const string &key) {
    size_t hash = GetBaseHash(key);

    for (size_t i = 0; i < table.size(); i++) {
        // Ячейка была и есть пуста
        if (table[hash].empty()) {
            assert(!deleted[hash]);
            return false;
        }

        // Сейчас в этой ячейке текущий ключ
        if ((table[hash] == key) && (!deleted[hash])) {
            deleted[hash] = true;
            return true;
        }

        // Раньше в этой ячейке был текущий ключ
        if ((table[hash] == key) && (deleted[hash])) {
            return false;
        }
        hash = GetNewHash(hash, i);
    }

    throw overflow_error("");
}


int main() {
    HashTable table(8, 5);
    char command = ' ';
    string value = " ";

    while (cin >> command >> value) {
        switch (command) {
            case '?':
                cout << (table.Has(value) ? "OK" : "FAIL") << endl;
                break;
            case '+':
                cout << (table.Add(value) ? "OK" : "FAIL") << endl;
                break;
            case '-':
                cout << (table.Remove(value) ? "OK" : "FAIL") << endl;
                break;
            default:
                cout << "FAIL" << endl;
        }
    }

    return 0;
}
