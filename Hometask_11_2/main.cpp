// Задача № 11. (5 баллов)
//
// № 11_2. Количество различных путей.
// Дан невзвешенный неориентированный граф. В графе может быть несколько
// кратчайших путей между какими-то вершинами. Найдите количество различных
// кратчайших путей между заданными вершинами. Требуемая сложность O(V + E).
//
// Формат входных данных:
// V: кол-во вершин (макс. 50000), E: кол-во ребер (макс. 200000), E пар
// реберных вершин, пара вершин u, w для запроса.
//
// Формат выходных данных:
// Количество кратчайших путей от u к w.


// Время работы: O(V + E)
// Потребляемая память: O(V + E),
// где V - количество вершин графа, E - количество ребер графа во входных данных


#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>


using std::cin;
using std::cout;
using std::vector;


class Graph {
private:
    struct Vertex {
        // Глубина текущей вершины во время BFS из начальной вершины
        int depth;
        // Количество путей до текущей вершины из начальной вершины
        int paths;

        Vertex() : depth(0), paths(0) {}
    };

    vector<vector<int>> vertices;

public:
    explicit Graph(int V) : vertices(V) {}

    void AddEdge(int from, int to);

    int CountShortestPaths(int from, int to) const;
};


// Среднее время работы: O(E / V),
// где V - количество вершин графа, E - количество ребер графа
void Graph::AddEdge(int from, int to) {
    // Ребро, находящееся в списке смежности, не добавляется повторно, т.е.
    // кратные ребра отсутствуют
    auto position = std::find(vertices[from].begin(), vertices[from].end(), to);
    if (position != vertices[from].end()) {
        return;
    }

    // Заполнение списка смежности
    vertices[from].push_back(to);
    vertices[to].push_back(from);
}


// Время работы: O(V + E),
// где V - количество вершин графа, E - количество ребер графа
int Graph::CountShortestPaths(int from, int to) const {
    // Хранит информацию про посещенные вершины
    vector<Vertex> states(vertices.size());
    states[from].paths = 1;

    std::queue<int> queue;
    queue.push(from);

    // BFS из начальной вершины
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int i : vertices[current]) {
            if (states[i].paths == 0) {
                // Посещение вершины в первый раз
                queue.push(i);
                states[i].depth = states[current].depth + 1;
                states[i].paths = states[current].paths;
            } else {
                // Посещение вершины не в первый раз
                // Глубина не может отличаться от текущей более чем на 1
                assert(abs(states[i].depth - states[current].depth) <= 1);

                if (states[i].depth == states[current].depth + 1) {
                    states[i].paths += states[current].paths;
                }
            }
        }
    }

    return states[to].paths;
}


int main() {
    int V = 0, E = 0;
    cin >> V >> E;

    Graph graph(V);

    int from = 0, to = 0;
    for (int i = 0; i < E; i++) {
        cin >> from >> to;
        graph.AddEdge(from, to);
    }

    int u = 0, w = 0;
    cin >> u >> w;
    cout << graph.CountShortestPaths(u, w);

    return 0;
}
