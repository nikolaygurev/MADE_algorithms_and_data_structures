// Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
//
// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение.
// Если команда pop* вызвана для пустой структуры данных, то ожидается “-1”.
//
// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали.
// Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO
//
// №1_3. Реализовать очередь с помощью двух стеков.
// Использовать стек, реализованный с помощью динамического буфера.


// Время работы: O(n)
// Потребляемая память: O(n)


#include <cassert>
#include <iostream>

using std::cin;
using std::cout;


class Stack {
private:
    const int default_size = 20; // Стартовый размер выделенного буфера
    int current_size; // Текущий размер выделенного буфера
    int *stack_ptr; // Указатель на начало выделенного буфера
    int head; // Номер верхнего элемента

    // Выделить буфер в 2 раза больше
    void extend_stack();

    // Выделить буфер в 2 раза меньше
    void narrow_down_stack();

public:
    explicit Stack();

    ~Stack();

    bool empty() const;

    void push(int value);

    int pop();
};

Stack::Stack() : current_size(default_size), head(-1) {
    stack_ptr = new int[current_size];
}

Stack::~Stack() {
    delete[] stack_ptr;
}

void Stack::extend_stack() {
    // Выделяем буфер в 2 раза больше
    int *new_ptr = new int[current_size * 2];

    // Копируем элементы из старого буфера в новый
    for (int i = 0; i < current_size; i++) {
        new_ptr[i] = stack_ptr[i];
    }

    // Удаляем старый буфер
    delete[] stack_ptr;

    stack_ptr = new_ptr;
    current_size *= 2;
}

void Stack::narrow_down_stack() {
    // Выделяем буфер в 2 раза меньше
    int *new_ptr = new int[current_size / 2];

    // Копируем элементы из старого буфера в новый
    for (int i = 0; i < current_size / 4; i++) {
        new_ptr[i] = stack_ptr[i];
    }

    // Удаляем старый буфер
    delete[] stack_ptr;

    stack_ptr = new_ptr;
    current_size /= 2;
}

// Время работы: O(1)
bool Stack::empty() const {
    return (head == -1);
}

// Амортизированное время работы: O(1)
void Stack::push(int value) {
    // Расширяем буфер в 2 раза, если в нём закончилось место
    if (head == current_size - 1) {
        extend_stack();
    }
    stack_ptr[++head] = value;
}

// Амортизированное время работы: O(1)
int Stack::pop() {
    int result = stack_ptr[head--];

    // Сужаем буфер в 2 раза, если в нём занято <= 25%, и
    // новый буфер будет не меньше размера по умолчанию (20)
    if ((head < current_size / 4) && (current_size >= default_size * 2)) {
        narrow_down_stack();
    }

    return result;
}


class Queue {
private:
    Stack left_stack;
    Stack right_stack;

public:
    Queue() : left_stack(), right_stack() {}

    bool empty() const;

    void push(int value);

    int pop();
};

// Время работы: O(1)
bool Queue::empty() const {
    return (left_stack.empty() && right_stack.empty());
}

// Амортизированное время работы: O(1)
void Queue::push(int value) {
    left_stack.push(value);
}

// Амортизированное время работы: O(1)
int Queue::pop() {
    assert(!empty());

    // Если в правом стеке нет элементов, переложим все элементы из левого стека в правый
    if (right_stack.empty()) {
        while (!left_stack.empty()) {
            right_stack.push(left_stack.pop());
        }
    }

    return right_stack.pop();
}

int main() {
    int n_commands = 0;
    cin >> n_commands;

    Queue queue;
    for (int i = 0; i < n_commands; ++i) {
        int command = 0;
        int value = 0;
        cin >> command >> value;
        assert((command == 2) || (command == 3));

        switch (command) {
            case 2:
                if (queue.empty()) {
                    if (value != -1) {
                        cout << "NO";
                        return 0;
                    }
                } else if (queue.pop() != value) {
                    cout << "NO";
                    return 0;
                }
                break;
            case 3:
                queue.push(value);
                break;
            default:
                cout << "NO";
                return 0;
        }
    }

    cout << "YES";
    return 0;
}
