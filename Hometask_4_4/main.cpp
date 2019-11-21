// Даны неотрицательные целые числа n, k и массив целых чисел из [0..10^9]
// размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число,
// которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
// Напишите нерекурсивный алгоритм.
//
// Требования к дополнительной памяти: O(n).
// Требуемое среднее время работы: O(n).
// Функцию Partition следует реализовать методом прохода двумя итераторами в
// одном направлении.
//
// №4_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца
// массива к началу.


// Среднее время работы: O(n)
// Потребляемая память: O(n)


#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>


using std::cin;
using std::cout;
using std::endl;
using std::pow;
using std::swap;
using std::vector;


std::random_device rd;
std::mt19937 generator(rd());


int GetRandomArrayIndex(int size) {
    // Возвращает случайное число из диапазона: [0; size)
    std::uniform_int_distribution<> dis(0, size - 1);

    return dis(generator);
}


// Время работы: O(right - left)
int PartitionForward(int *array, int left, int right) {
    // Случайно выбирает опорный элемент, ставит его на правильную позицию в
    // отсортированном по неубыванию массиве
    // Проход двумя итераторами от начала массива к концу
    assert(left < right);

    array = array + left;
    int size = right - left;
    int pivot = GetRandomArrayIndex(size);
    swap(array[pivot], array[size - 1]);

    int i = 0, j = 0;
    // В массиве слева направо лежат:
    // - элементы не больше опорного: [0; i)
    // - элементы строго больше опорного: [i; j)
    // - нерассмотренные элементы: [j; size - 1)
    // - опорный элемент на позиции (size - 1)

    while (j < size - 1) {
        if (array[j] <= array[size - 1]) {
            swap(array[i], array[j]);
            i++;
        }
        j++;
    }
    swap(array[i], array[size - 1]);

    return left + i;
}


// Время работы: O(right - left)
int PartitionBackward(int *array, int left, int right) {
    // Случайно выбирает опорный элемент, ставит его на правильную позицию в
    // отсортированном по неубыванию массиве
    // Проход двумя итераторами от конца массива к началу
    assert(left < right);

    array = array + left;
    int size = right - left;
    int pivot = GetRandomArrayIndex(size);
    swap(array[pivot], array[0]);

    int i = size - 1, j = size - 1;
    // В массиве слева направо лежат:
    // - опорный элемент на позиции 0
    // - нерассмотренные элементы: [1; j]
    // - элементы не больше опорного: [j + 1; i]
    // - элементы строго больше опорного: [i + 1; size - 1]

    while (j >= 1) {
        if (array[j] > array[0]) {
            swap(array[i], array[j]);
            i--;
        }
        j--;
    }
    swap(array[i], array[0]);

    return left + i;
}


// Среднее время работы: O(size)
int FindOrderStatRecursive(int *array, int size, int k) {
    assert(0 <= k && k < size);

    int pivot = PartitionBackward(array, 0, size);
    assert(0 <= pivot && pivot < size);

    int kth_stat = 0;
    if (pivot == k) {
        kth_stat = array[pivot];
    } else if (k < pivot) {
        // Ищем в левой половине: [0; pivot)
        kth_stat = FindOrderStatRecursive(array, pivot, k);
    } else {
        // Ищем в правой половине: [pivot + 1; size)
        int bias = pivot + 1;
        kth_stat = FindOrderStatRecursive(array + bias, size - bias, k - bias);
    }

    return kth_stat;
}


// Среднее время работы: O(size)
int FindOrderStatIterative(int *array, int size, int k) {
    assert(0 <= k && k < size);

    int left = 0;
    int right = size;

    while (true) {
        int pivot = PartitionBackward(array, left, right);
        assert(left <= pivot && pivot < right);

        if (pivot == k) {
            return array[pivot];
        } else if (k < pivot) {
            // Ищем в левой половине: [left; pivot)
            right = pivot;
        } else {
            // Ищем в правой половине: [pivot + 1; right)
            left = pivot + 1;
        }
    }
}


int FindOrderStatDefault(int *array, int size, int k) {
    assert(0 <= k && k < size);
    std::nth_element(array, array + k, array + size);

    return array[k];
}


void Test() {
    const int SIZES[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 50, 100, 200};
    const int N_ITER = pow(10, 4);
    std::uniform_int_distribution<> test_values_dist(0, pow(10, 9));

    // Для каждой длины входного массива из SIZES
    for (unsigned long i = 0; i < sizeof(SIZES) / sizeof(SIZES[0]); i++) {
        const int SIZE = SIZES[i];
        cout << "Test for size: " << SIZE;

        std::uniform_int_distribution<> test_k_dist(0, SIZE - 1);

        // N_ITER раз повторяем
        for (int j = 0; j < N_ITER; j++) {
            // Создаём случайное k
            int k = test_k_dist(generator);

            // Создаём 3 одинаковых случайных вектора размера SIZE
            int value = 0;
            vector<int> values0 = {};
            vector<int> values1 = {};
            vector<int> values2 = {};
            for (int h = 0; h < SIZE; h++) {
                value = test_values_dist(generator);
                values0.push_back(value);
                values1.push_back(value);
                values2.push_back(value);
            }

            // Проверяем, что дефолтная, итеративная и рекурсивная функции
            // возвращают одинаковые значения
            int res0 = FindOrderStatDefault(values0.data(), SIZE, k);
            int res1 = FindOrderStatRecursive(values1.data(), SIZE, k);
            int res2 = FindOrderStatIterative(values2.data(), SIZE, k);
            assert(res0 == res1);
            assert(res0 == res2);
        }
        cout << " - finished" << endl;
    }
}


int main() {
//    Test();

    int n = 0;
    int k = 0;
    cin >> n >> k;

    int value = 0;
    vector<int> values = {};
    for (int i = 0; i < n; i++) {
        cin >> value;
        values.push_back(value);
    }

    int order_statistic = FindOrderStatIterative(values.data(), n, k);
    cout << order_statistic;

    return 0;
}
