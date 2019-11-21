#include <iostream>


using std::cout;
using std::endl;
using std::swap;


struct Point {
    int x;
    int y;

    bool operator<(const Point &other) const;
};


bool Point::operator<(const Point &other) const {
    return y < other.y;
}


// Использует для сравнения оператор <
void BubbleSort(Point *arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j + 1] < arr[j]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


// Функция, сравнивающая точки по расстоянию до начала координат
bool CompareByDistToZero(const Point &first, const Point &second) {
    return (first.x * first.x + first.y * first.y) <
           (second.x * second.x + second.y * second.y);
}


// Использует для сравнения функцию конкретного вида
void BubbleSort(Point *arr, int size,
                bool (*compare)(const Point &, const Point &)) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (compare(arr[j + 1], arr[j])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


// Использует для сравнения что-то шаблонное, имеющее ()
template<typename TCompare>
void BubbleSortT(Point *arr, int size, const TCompare &compare) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (compare(arr[j + 1], arr[j])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


// Функтор, сравнивающий точки по расстоянию до center
class ComparerByDistToCenter {
private:
    Point center;
public:
    explicit ComparerByDistToCenter(const Point &_center) : center(_center) {};

    bool operator()(const Point &first, const Point &second) const;
};


bool ComparerByDistToCenter::operator()(const Point &first,
                                        const Point &second) const {
    const int first_diff_x = first.x - center.x;
    const int first_diff_y = first.y - center.y;
    const int second_diff_x = second.x - center.x;
    const int second_diff_y = second.y - center.y;

    return (first_diff_x * first_diff_x + first_diff_y * first_diff_y) <
           (second_diff_x * second_diff_x + second_diff_y * second_diff_y);
}


int main() {
    Point arr[]{
            Point{3, 5},
            Point{0, 0},
            Point{4, 2},
            Point{2, 6},
            Point{1, 2}
    };

//    BubbleSort(arr, 5);
//    BubbleSort(arr, 5, CompareByDistToZero);
//    BubbleSortT(arr, 5, CompareByDistToZero);
//    BubbleSortT(arr, 5, ComparerByDistToCenter(Point{3, 3}));

    // Две точки сравниваются по расстоянию до заданной точки при помощи
    // lambda-функции
    const Point center{3, 3};
    BubbleSortT(arr, 5, [&center](const Point &first, const Point &second) {
        const int first_diff_x = first.x - center.x;
        const int first_diff_y = first.y - center.y;
        const int second_diff_x = second.x - center.x;
        const int second_diff_y = second.y - center.y;

        return (first_diff_x * first_diff_x + first_diff_y * first_diff_y) <
               (second_diff_x * second_diff_x + second_diff_y * second_diff_y);
    });

    for (int i = 0; i < 5; ++i) {
        cout << arr[i].x << " " << arr[i].y << endl;
    }

    return 0;
}
