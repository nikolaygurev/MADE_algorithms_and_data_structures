#include <cassert>
#include <iostream>


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
    bool empty() const;

    // Добавление элемента
    void push(int value);

    // Извелечение
    int pop();
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

bool Queue::empty() const {
    assert((head == nullptr) == (tail == nullptr));

    return head == nullptr;
}

void Queue::push(int value) {
    if (empty()) {
        head = tail = new QueueNode{value, nullptr};
    } else {
        tail->next = new QueueNode{value, nullptr};
        tail = tail->next;
    }
}

int Queue::pop() {
    assert(!empty());
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
    int commands_count = 0;
    std::cin >> commands_count;

    Queue queue;
    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;

        if (command == 3) {
            queue.push(value);
        } else if (command == 2) {
            if (queue.empty()) {
                if (value != -1) {
                    std::cout << "NO";
                    return 0;
                }
            } else if (queue.pop() != value) {
                std::cout << "NO";
                return 0;
            }
        }
    }

    std::cout << "YES";
    return 0;
}
