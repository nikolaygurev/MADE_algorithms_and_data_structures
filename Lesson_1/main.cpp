#include <cassert>
#include <iostream>


using std::cin;
using std::cout;


// Узел односвязного списка
struct QueueNode {
    int value;
    QueueNode *next;

    QueueNode(int val, QueueNode *ptr) : value(val), next(ptr) {}
};


class Queue {
private:
    QueueNode *head;
    QueueNode *tail;

public:
    Queue() : head(nullptr), tail(nullptr) {}

    ~Queue();

    // Проверка очереди на пустоту
    bool Empty() const;

    // Добавление элемента
    void Push(int value);

    // Извелечение
    int Pop();
};


Queue::~Queue() {
    while (head) {
        QueueNode *temp = head->next;
        delete head;
        head = temp;

//        // Альтернативный вариант:
//        QueueNode *to_delete = head;
//        head = head->next;
//        delete to_delete;
    }
}


bool Queue::Empty() const {
    assert((head == nullptr) == (tail == nullptr));

    return head == nullptr;
}


void Queue::Push(int value) {
    if (Empty()) {
        head = tail = new QueueNode{value, nullptr};
    } else {
        tail->next = new QueueNode{value, nullptr};
        tail = tail->next;
    }
}


int Queue::Pop() {
    assert(!Empty());
    int result = head->value;

    if (head == tail) {
        // Остался один элемент
        delete head;
        head = tail = nullptr;
    } else {
        QueueNode *temp = head->next;
        delete head;
        head = temp;
    }

    return result;
}


int main() {
    int n = 0;
    cin >> n;

    Queue queue;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int value = 0;
        cin >> command >> value;

        if (command == 3) {
            queue.Push(value);
        } else if (command == 2) {
            if (queue.Empty()) {
                if (value != -1) {
                    cout << "NO";
                    return 0;
                }
            } else if (queue.Pop() != value) {
                cout << "NO";
                return 0;
            }
        }
    }
    cout << "YES";

    return 0;
}
