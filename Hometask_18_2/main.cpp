// Задача № 18. Построение выпуклой оболочки. (5 баллов)
//
// Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого
// множества и вычислите ее периметр. Обратите внимание: три и более точки из
// множества могут лежать на одной прямой.
//
// № 18_2. С помощью алгоритма Джарвиса.
//
// Формат входных данных:
// Количество точек, далее на каждой строке координаты точек x, y. Общее
// количество точек не превосходит 100000. Координаты точек - числа с плавающей
// точкой в диапазоне [-1000, 1000].
//
// Формат выходных данных:
// Периметр выпуклой оболочки.


// Среднее время работы: O(n * log(n))
// Потребляемая память: O(n),
// где n - количество точек во входных данных


#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>


using std::cin;
using std::cout;
using std::pow;
using std::vector;


const double eps = 1e-10;


struct Point {
    double x;
    double y;

    Point(double x_, double y_) : x(x_), y(y_) {}
};


// Время работы: O(1)
double DistBetween(const Point &A, const Point &B) {
    // Расстояние между точками A и B в двумерном пространстве
    return std::sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
}


// Время работы: O(1)
double CrossProduct(const Point &A, const Point &B,
                    const Point &C, const Point &D) {
    // Векторное произведение векторов AB и CD в двумерном пространстве
    return (B.x - A.x) * (D.y - C.y) - (D.x - C.x) * (B.y - A.y);
}


// Среднее время работы: O(n * log(n)), где n = points.size()
void SearchForConvexHull(const vector<Point> &points,
                         vector<Point> &hull_points) {
    assert(points.size() >= 2);

    // Ищем стартовую точку - самую правую среди самых нижних
    int start_i = 0;
    for (int i = 1; i < points.size(); i++) {
        if ((points[i].y < points[start_i].y) ||
            ((points[i].y == points[start_i].y) &&
             (points[i].x > points[start_i].x))) {
            start_i = i;
        }
    }

    int cur = start_i;
    do {
        hull_points.push_back(points[cur]);

        // Гарантированно отличается от cur
        int best = (cur + 1) % static_cast<int>(points.size());

        // Перебираем все точки и ищем наилучшую
        for (int i = 0; i < points.size(); i++) {
            double cross_product = CrossProduct(points[cur], points[best],
                                                points[cur], points[i]);
            if (cross_product < -eps) {
                // Векторное произведение [cur_best, cur_i] - отрицательное =>
                // угол вращения против часовой стрелки от cur_best к cur_i
                // отрицательный => cur_i является оболочкой для cur_best
                assert(i != best);
                best = i;
            } else if ((-eps <= cross_product) && (cross_product <= eps)) {
                // Векторное произведение [cur_best, cur_i] равно нулю => угол
                // вращения против часовой стрелки от cur_best к cur_i равен
                // нулю (не 180 градусов, т.к. в случае нескольких точек на
                // одной прямой мы всегда находимся в крайней) => оболочкой
                // является более длинный вектор
                double dist_best = DistBetween(points[cur], points[best]);
                double dist_i = DistBetween(points[cur], points[i]);

                if (dist_i > dist_best) {
                    best = i;
                }
            }
        }
        cur = best;
    } while (cur != start_i);
}


// Время работы: O(hull_points.size())
double Perimeter(vector<Point> hull_points) {
    // Добавляем стартовую точку в конец для того, чтобы замкнуть оболочку
    hull_points.push_back(hull_points[0]);
    double perimeter = 0;

    for (int i = 0; i < hull_points.size() - 1; i++) {
        perimeter += DistBetween(hull_points[i], hull_points[i + 1]);
    }

    return perimeter;
}


int main() {
    int n = 0;
    cin >> n;

    vector<Point> points;
    for (int i = 0; i < n; i++) {
        double x = 0, y = 0;
        cin >> x >> y;
        points.emplace_back(x, y);
    }

    vector<Point> hull_points;
    SearchForConvexHull(points, hull_points);
    cout << std::setprecision(10) << Perimeter(hull_points);

    return 0;
}
