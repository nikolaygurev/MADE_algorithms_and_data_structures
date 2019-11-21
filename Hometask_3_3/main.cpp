// 3_3. Количество инверсий.
// Дана последовательность целых чисел из диапазона (-10^9 .. 10^9). Длина
// последовательности не больше 10^6. Числа записаны по одному в строке.
// Количество чисел не указано.
//
// Пусть количество элементов n, и числа записаны в массиве a = a[i], i из
// [0...n-1]. Требуется напечатать количество таких пар индексов (i,j) из
// [0...n-1], что (i < j и a[i] > a[j]).


// Время работы: O(n * log(n))
// Потребляемая память: O(n)


#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>


using std::cin;
using std::cout;
using std::memcpy;
using std::vector;


// Время работы: O(right - left)
int64_t MergeWithInvCount(int *array, int left, int mid, int right) {
    int64_t n_inversions = 0;
    int *temp_array = new int[right - left];
    int l_it = 0;
    int r_it = 0;

    while (left + l_it < mid && mid + r_it < right) {
        if (array[left + l_it] <= array[mid + r_it]) {
            temp_array[l_it + r_it] = array[left + l_it];
            l_it++;
        } else {
            temp_array[l_it + r_it] = array[mid + r_it];
            r_it++;
            // Добавляем число инверсий текущего элемента из правого подмассива
            // со всеми оставшимися элементами левого подмассива
            n_inversions += (mid - left - l_it);
        }
    }

    while (left + l_it < mid) {
        temp_array[l_it + r_it] = array[left + l_it];
        l_it++;
    }

    while (mid + r_it < right) {
        temp_array[l_it + r_it] = array[mid + r_it];
        r_it++;
    }

    assert(l_it + r_it == right - left);
    memcpy(array + left, temp_array, sizeof(int) * (right - left));
    delete[] temp_array;

    return n_inversions;
}


// Время работы: O(size * log(size))
int64_t MergeSortIterativeWithInvCount(int *array, int size) {
    int64_t n_inversions = 0;

    // i - текущий размер подмассивов
    // j - левая граница двух объединяемых подмассивов
    for (int i = 1; i < size; i *= 2) {
        for (int j = 0; j < size - i; j += 2 * i) {
            const int right = (j + 2 * i <= size) ? j + 2 * i : size;
            n_inversions += MergeWithInvCount(array, j, j + i, right);
        }
    }

    return n_inversions;
}


int main() {
    int value = 0;
    vector<int> values = {};
    while (!cin.eof() && cin >> value) {
        values.push_back(value);
    }

    int64_t n_inversions = MergeSortIterativeWithInvCount(values.data(),
                                                          values.size());
    cout << n_inversions;

    return 0;
}
