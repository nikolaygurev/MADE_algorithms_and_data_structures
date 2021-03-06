// Решение всех задач данного раздела предполагает использование кучи.
//
// 2_3. Тупики.
// На вокзале есть некоторое количество тупиков, куда прибывают электрички. Этот
// вокзал является их конечной станцией. Дано расписание движения электричек,
// в котором для каждой электрички указано время ее прибытия, а также время
// отправления в следующий рейс. Электрички в расписании упорядочены по времени
// прибытия.
// Когда электричка прибывает, ее ставят в свободный тупик с минимальным
// номером. При этом, если электричка из какого-то тупика отправилась в момент
// времени X, то электричку, которая прибывает в момент времени X, в этот тупик
// ставить нельзя, а электричку, прибывающую в момент времени X+1 — можно.
// В данный момент на вокзале достаточное количество тупиков для работы по
// расписанию. Напишите программу, которая по данному расписанию определяет,
// какое минимальное количество тупиков требуется для работы вокзала.
//
// Формат входных данных.
// Вначале вводится n - количество электричек в расписании. Затем вводится n
// строк для каждой электрички, в строке - время прибытия и время отправления.
// Время - натуральное число от 0 до 10^9. Строки в расписании упорядочены по
// времени прибытия.
//
// Формат выходных данных.
// Натуральное число - минимальное количеством тупиков.
// Максимальное время: 50мс, память: 5Мб.


// Время работы: O(n * log(n))
// Потребляемая память: O(n)


#include <algorithm>
#include <cassert>
#include <iostream>


using std::cin;
using std::cout;
using std::swap;


class MinHeap {
private:
    const int default_size = 20; // Стартовый размер выделенного буфера
    int current_size; // Текущий размер выделенного буфера
    int *heap_ptr; // Указатель на начало выделенного буфера
    int last; // Номер последнего элемента кучи

    // Выделить буфер в 2 раза больше, чем used_size
    void ResizeHeap(int used_size);

    // Добавить элемент в конец кучи
    void PushBack(int value);

    // Извлечь элемент из корня кучи
    int PopFront();

    // Просеять элемент вверх
    void SiftUp(int pos);

    // Просеять элемент вниз
    void SiftDown(int pos);

public:
    MinHeap();

    // Конструктор от массива
    MinHeap(int *arr, int arr_size, bool is_inplace);

    ~MinHeap();

    bool Empty() const;

    int Top() const;

    int Size() const;

    // Добавить элемент в кучу
    void Push(int value);

    // Извлечь элемент из кучи
    int Pop();
};


MinHeap::MinHeap() : current_size(default_size), last(-1) {
    heap_ptr = new int[current_size];
}


// Амортизированное время работы: O(arr_size)
MinHeap::MinHeap(int *arr, int arr_size, bool is_inplace) :
        current_size(arr_size), last(current_size - 1) {
    if (is_inplace) {
        heap_ptr = arr;
    } else {
        heap_ptr = new int[current_size];

        for (int i = 0; i < current_size; i++) {
            heap_ptr[i] = arr[i];
        }
    }

    for (int i = current_size / 2 - 1; i >= 0; i--) {
        SiftDown(i);
    }
}


MinHeap::~MinHeap() {
    delete[] heap_ptr;
}


void MinHeap::ResizeHeap(int used_size) {
    // Выделяем буфер в 2 раза больше, чем used_size
    int *new_ptr = new int[used_size * 2];

    // Копируем элементы из старого буфера в новый
    for (int i = 0; i < used_size; i++) {
        new_ptr[i] = heap_ptr[i];
    }

    // Удаляем старый буфер
    delete[] heap_ptr;

    heap_ptr = new_ptr;
    current_size = used_size * 2;
}


// Амортизированное время работы: O(1)
void MinHeap::PushBack(int value) {
    // Расширяем буфер в 2 раза, если в нём закончилось место
    if (last == current_size - 1) {
        ResizeHeap(current_size);
    }
    heap_ptr[++last] = value;
}


// Амортизированное время работы: O(1)
int MinHeap::PopFront() {
    int result = Top();
    heap_ptr[0] = heap_ptr[last--];

    // Сужаем буфер в 2 раза, если в нём занято <= 25%, и
    // новый буфер будет не меньше размера по умолчанию (20)
    if ((last < current_size / 4) && (current_size >= default_size * 2)) {
        ResizeHeap(current_size / 4);
    }

    return result;
}


// Время работы: O(log(n))
void MinHeap::SiftUp(int pos) {
    // int((0 - 1) / 2) = 0, поэтому цикл гарантировано остановится при pos = 0
    // (возможно остановится раньше)
    while (heap_ptr[pos] < heap_ptr[(pos - 1) / 2]) {
        swap(heap_ptr[pos], heap_ptr[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
    }
}


// Время работы: O(log(n))
void MinHeap::SiftDown(int pos) {
    while (2 * pos + 1 <= last) {
        int left = 2 * pos + 1;
        int right = 2 * pos + 2;

        // Изначально считаем, что наименьший из детей - левый
        int min_child = left;

        // Если правый ребёнок существует, и он меньше левого, то
        // наименьший из детей - правый
        if ((right <= last) && (heap_ptr[right] < heap_ptr[left])) {
            min_child = right;
        }

        // Если родитель меньше, чем наименьший из детей, то
        // свойство мин. кучи восстановлено, и операция SiftDown закончена
        if (heap_ptr[pos] <= heap_ptr[min_child]) {
            break;
        }

        swap(heap_ptr[pos], heap_ptr[min_child]);
        pos = min_child;
    }
}


// Время работы: O(1)
bool MinHeap::Empty() const {
    return (last == -1);
}


// Время работы: O(1)
int MinHeap::Top() const {
    assert(!Empty());

    return heap_ptr[0];
}


// Время работы: O(1)
int MinHeap::Size() const {
    return (last + 1);
}


// Амортизированное время работы: O(log(n))
void MinHeap::Push(int value) {
    PushBack(value);
    SiftUp(last);
}


// Амортизированное время работы: O(log(n))
int MinHeap::Pop() {
    int result = PopFront();
    SiftDown(0);

    return result;
}


int main() {
    int n = 0;
    cin >> n;

    MinHeap min_heap;
    int max_trains = 0;

    for (int i = 0; i < n; i++) {
        int train_in = 0;
        int train_out = 0;
        cin >> train_in >> train_out;
        assert((train_in >= 0) && (train_out >= 0) && (train_in <= train_out));

        // Убираем из кучи поезда, которые уехали до момента прибытия
        // текущего поезда
        while (!min_heap.Empty() && (min_heap.Top() < train_in)) {
            min_heap.Pop();
        }

        min_heap.Push(train_out);

        // Запоминаем текущее кол-во поездов
        if (min_heap.Size() > max_trains) {
            max_trains = min_heap.Size();
        };
    }

    cout << max_trains;

    return 0;
}
