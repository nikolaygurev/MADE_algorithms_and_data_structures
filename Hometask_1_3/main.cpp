// Во всех задачах из следующего списка следует написать структуру данных,
// обрабатывающую команды Push* и Pop*.
//
// Формат входных данных.
// В первой строке количество команд n, n <= 1000000. Каждая команда задаётся,
// как 2 целых числа: a b.
// a = 1 - Push front
// a = 2 - Pop front
// a = 3 - Push back
// a = 4 - Pop back
// Для очереди используются команды 2 и 3. Для дека используются все четыре
// команды. Если дана команда Pop*, то число b - ожидаемое значение. Если
// команда Pop* вызвана для пустой структуры данных, то ожидается “-1”.
//
// Формат выходных данных.
// Требуется напечатать YES, если все ожидаемые значения совпали. Иначе, если
// хотя бы одно ожидание не оправдалось, нужно напечатать NO.
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

    // Выделить буфер в 2 раза больше, чем used_size
    void ResizeStack(int used_size);

public:
    Stack();

    ~Stack();

    bool Empty() const;

    void Push(int value);

    int Pop();
};


Stack::Stack() : current_size(default_size), head(-1) {
    stack_ptr = new int[current_size];
}


Stack::~Stack() {
    delete[] stack_ptr;
}


void Stack::ResizeStack(int used_size) {
    // Выделяем буфер в 2 раза больше, чем used_size
    int *new_ptr = new int[used_size * 2];

    // Копируем элементы из старого буфера в новый
    for (int i = 0; i < used_size; i++) {
        new_ptr[i] = stack_ptr[i];
    }

    // Удаляем старый буфер
    delete[] stack_ptr;

    stack_ptr = new_ptr;
    current_size = used_size * 2;
}


// Время работы: O(1)
bool Stack::Empty() const {
    return (head == -1);
}


// Амортизированное время работы: O(1)
void Stack::Push(int value) {
    // Расширяем буфер в 2 раза, если в нём закончилось место
    if (head == current_size - 1) {
        ResizeStack(current_size);
    }
    stack_ptr[++head] = value;
}


// Амортизированное время работы: O(1)
int Stack::Pop() {
    int result = stack_ptr[head--];

    // Сужаем буфер в 2 раза, если в нём занято <= 25%, и
    // новый буфер будет не меньше размера по умолчанию (20)
    if ((head < current_size / 4) && (current_size >= default_size * 2)) {
        ResizeStack(current_size / 4);
    }

    return result;
}


class Queue {
private:
    Stack left_stack;
    Stack right_stack;

public:
    Queue() : left_stack(), right_stack() {}

    bool Empty() const;

    void Push(int value);

    int Pop();
};


// Время работы: O(1)
bool Queue::Empty() const {
    return (left_stack.Empty() && right_stack.Empty());
}


// Амортизированное время работы: O(1)
void Queue::Push(int value) {
    left_stack.Push(value);
}


// Амортизированное время работы: O(1)
int Queue::Pop() {
    assert(!Empty());

    // Если в правом стеке нет элементов,
    // переложим все элементы из левого стека в правый
    if (right_stack.Empty()) {
        while (!left_stack.Empty()) {
            right_stack.Push(left_stack.Pop());
        }
    }

    return right_stack.Pop();
}


int main() {
    int n = 0;
    cin >> n;

    Queue queue;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int value = 0;
        cin >> command >> value;
        assert((command == 2) || (command == 3));

        switch (command) {
            case 2:
                if (queue.Empty()) {
                    if (value != -1) {
                        cout << "NO";
                        return 0;
                    }
                } else if (queue.Pop() != value) {
                    cout << "NO";
                    return 0;
                }
                break;
            case 3:
                queue.Push(value);
                break;
            default:
                cout << "NO";
                return 0;
        }
    }
    cout << "YES";

    return 0;
}
