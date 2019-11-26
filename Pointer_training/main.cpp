#include <iostream>


using std::cout;
using std::endl;


void CoutArray(int *arr, int size) {
    cout << "Адрес массива (первого элемента) в памяти "
         << "(значение указателя arr): " << arr << endl << endl;
    for (int i = 0; i < size; i++) {
        cout << "Элемент с индексом: " << i << endl;
        cout << "Адрес в памяти: "
             << "(arr + " << i << ") = " << arr + i << endl;
        cout << "Непрямой доступ (разыменование): "
             << "*(arr + " << i << ") = " << *(arr + i) << endl;
        cout << "Прямой доступ (по индексу): "
             << "arr[" << i << "] = " << arr[i] << endl << endl;
    }
}


void ArrayOfInt() {
    const int SIZE = 4;
    // Переменная arr типа указатель на int содержит адрес начала
    // выделенного у операционной системы блока памяти (динамического массива)
    // на 4 элемента типа int
    int *arr = new int[SIZE];

    arr[0] = 10;
    arr[2] = 12;
    arr[3] = 13;

    // arr[1] не заполнен, в нём мусор
    // arr[4] находится за пределами выделенного блока памяти,
    // к нему некорректно обращаться
    cout << "Размер выделенного блока памяти: " << SIZE << endl;
    CoutArray(arr, SIZE + 1);

    // В переменную var0 копируется значение arr[0]
    int var0 = arr[0];
    // Изменение var0 не влияет на arr[0]
    var0++;
    cout << "var0 изменилось: " << var0 << endl;
    cout << "arr[0] не изменилось: " << arr[0] << endl << endl;

    // Переменная var2 ссылается на значение arr[2]
    int &var2 = arr[2];
    // Изменение var2 влечет за собой изменение arr[2]
    var2 = var2 + 42;
    cout << "var2 изменилось: " << var2 << endl;
    cout << "arr[2] тоже изменилось: " << arr[2] << endl << endl;

    // Нужно вернуть операционной системе выделенную память, адрес которой
    // записан в указателе arr
    delete[] arr;

    // Нельзя использовать указатель на освобожденную память, хотя в ней могут
    // остаться значения, находившиеся в ней до вызова delete
    CoutArray(arr, SIZE + 1);
}


void PointerToInt() {
    int a = 5;
    int *ptr = &a;

    cout << "a = " << a << endl;
    cout << "ptr = " << ptr << endl;
    cout << "*ptr = " << *ptr << endl;
}


int main() {
    ArrayOfInt();
//    PointerToInt();

    return 0;
}
