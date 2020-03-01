// Задача № 14. Минимальное остовное дерево. (5 баллов)
//
// Дан неориентированный связный граф. Требуется найти вес минимального
// остовного дерева в этом графе.
//
// № 14_1. С помощью алгоритма Прима.
//
// Формат входных данных:
// V: кол-во вершин, E: кол-во ребер, E взвешенных ребер (вершина, вершина, вес)
//
// Формат выходных данных:
// Вес минимального остовного дерева


// Время работы: O(E * log(V)),
// где V - количество вершин графа, E - количество ребер графа


#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>


using std::cin;
using std::cout;
using std::vector;


const int INT_INF = std::numeric_limits<int>::max();
typedef std::pair<int, int> pair;


class Graph {
private:
    vector<vector<pair>> vertices;

public:
    explicit Graph(int V) : vertices(V) {}

    void AddEdge(int from, int to, int weight);

    int FindMST() const;
};


// Время работы: O(1)
void Graph::AddEdge(int from, int to, int weight) {
    vertices[from].push_back(pair{to, weight});
    vertices[to].push_back(pair{from, weight});
}


// Время работы: O(E * log(V)),
// где V - количество вершин графа, E - количество ребер графа
int Graph::FindMST() const {
    // Для каждой вершины в векторах содержится информация:
    // weights - минимальное расстояние от вершины до построенной части MST
    // in_mst - используется ли вершина в MST

    vector<int> weights(vertices.size(), INT_INF);
    vector<bool> in_mst(vertices.size(), false);

    // set содержит просмотренные вершины, которые еще не добавлены в MST
    // В списке смежности вершины и в set хранятся пары (номер вершины, вес)
    auto comparator = [](pair a, pair b) { return a.second <= b.second; };
    std::set<pair, decltype(comparator)> set(comparator);

    // 0 - начальная вершина, имеет вес 0
    int start_vertex = 0;
    weights[start_vertex] = 0;
    set.emplace(start_vertex, weights[start_vertex]);

    while (!set.empty()) {
        int current = set.begin()->first;
        set.erase(set.begin());
        in_mst[current] = true;

        for (const auto &i : vertices[current]) {
            int label = i.first;
            int weight = i.second;

            if ((!in_mst[label]) && (weight < weights[label])) {
                set.erase(pair{label, weights[label]});
                weights[label] = weight;
                set.emplace(label, weights[label]);
            }
        }
    }

    // Теперь weights больше не содержит INT_INF
    assert(std::count(weights.begin(), weights.end(), INT_INF) == 0);

    int result = 0;
    for (int i : weights) {
        result += i;
    }

    return result;
}


int main() {
    int V = 0, E = 0;
    cin >> V >> E;

    Graph graph(V);

    int from = 0, to = 0, weight = 0;
    for (int i = 0; i < E; i++) {
        cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
    }

    cout << graph.FindMST();

    return 0;
}
