#include <cassert>
#include <iostream>
#include <string>
#include <vector>


using std::cin;
using std::cout;
using std::endl;
using std::move;
using std::string;
using std::vector;


class HashTable {
private:
    struct HashTableNode {
        string key;
        HashTableNode *next = nullptr;

        explicit HashTableNode(string key_) : key(move(key_)) {}
    };

    vector<HashTableNode *> table;

    size_t GetHash(const string &key) const;

public:
    explicit HashTable(size_t initial_size);

    ~HashTable();

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


// Создается вектор размера initial_size, все значения равны nullptr
HashTable::HashTable(size_t initial_size) : table(initial_size, nullptr) {}


HashTable::~HashTable() {
    for (HashTableNode *head : table) {
        while (head) {
            HashTableNode *next = head->next;
            delete head;
            head = next;
        }
    }
}


size_t HashTable::GetHash(const string &key) const {
    assert(!key.empty());

    return key[0] % table.size();
}


bool HashTable::Has(const string &key) const {
    const size_t hash = GetHash(key);
    for (auto head = table[hash]; head != nullptr; head = head->next) {
        if (head->key == key) {
            return true;
        }
    }

    return false;
}


bool HashTable::Add(const string &key) {
    const size_t hash = GetHash(key);
    for (auto head = table[hash]; head != nullptr; head = head->next) {
        if (head->key == key) {
            return false;
        }
    }
    auto *new_node = new HashTableNode(key);
    new_node->next = table[hash];
    table[hash] = new_node;

    return true;
}


bool HashTable::Remove(const string &key) {
    const size_t hash = GetHash(key);
    HashTableNode *&head = table[hash];

    if (!head) {
        return false;
    }

    if (head->key == key) {
        HashTableNode *to_delete = head;
        head = head->next;
        delete to_delete;
        return true;
    }

    HashTableNode *parent = head;
    for (; parent->next != nullptr; parent = parent->next) {
        if (parent->next->key == key) {
            HashTableNode *to_delete = parent->next;
            parent->next = parent->next->next;
            delete to_delete;
            return true;
        }
    }

    return false;
}


int main() {
    HashTable table(100);
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
