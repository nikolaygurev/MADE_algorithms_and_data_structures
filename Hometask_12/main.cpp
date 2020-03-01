// Задача № 12. Мосты. (5 баллов)
//
// Ребро неориентированного графа называется мостом, если удаление этого ребра
// из графа увеличивает число компонент связности. Дан неориентированный граф,
// требуется найти в нем все мосты.
//
// Формат входных данных:
// Первая строка входного файла содержит два целых числа V и E - количество
// вершин и ребер графа соответственно (1 <= V <= 20000, 1 <= E <= 200000).
// Следующие m строк содержат описание ребер по одному на строке. Ребро номер i
// описывается двумя натуральными числами bi, ei - номерами концов ребра
// (1 <= bi, ei <= V).
//
// Формат выходных данных:
// Первая строка выходного файла должна содержать одно натуральное число b -
// количество мостов в заданном графе. На следующей строке выведите b целых
// чисел - номера ребер, которые являются мостами, в возрастающем порядке. Ребра
// нумеруются с единицы в том порядке, в котором они заданы во входном файле.


// Время работы: O(V + E)
// Потребляемая память: O(V + E),
// где V - количество вершин графа, E - количество ребер графа во входных данных


#include <algorithm>
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


using std::fstream;
using std::min;
using std::string;
using std::unordered_map;
using std::vector;


typedef std::pair<int, int> pair;
typedef unordered_map<int, pair>::const_iterator map_iter;


struct IInputStream {
    fstream fs;

    explicit IInputStream(const string &filename) {
        fs.open(filename, fstream::in);
    }

    ~IInputStream() {
        fs.close();
        fs.clear();
    }

    pair Read() {
        int from = 0, to = 0;
        fs >> from >> to;

        return pair{from, to};
    }
};


struct IOutputStream {
    fstream fs;

    explicit IOutputStream(const string &filename) {
        fs.open(filename, fstream::out | fstream::trunc);
    }

    ~IOutputStream() {
        fs.close();
        fs.clear();
    }

    void Write(int value) {
        fs << value;
    }

    void Write(const string &value) {
        fs << value;
    }
};


class Graph {
private:
    // Количество вершин
    int V;
    // Список смежности:
    // Индекс вектора - номер вершины
    // Ключ unordered_map - номер смежной вершины
    // Значение unordered_map - пара {количество повторений текущего ребра
    // (кратные ребра), номер, с которым текущее ребро пришло в последний раз}
    vector<unordered_map<int, pair>> vertices;

    void BridgeDFSRecursive(int cur, vector<bool> &visited,
                            vector<int> &parent, vector<int> &disc_time,
                            vector<int> &low_time, vector<int> &bridges,
                            int &time) const;

    void BridgeDFSIterative(int cur, vector<bool> &visited,
                            vector<int> &parent, vector<int> &disc_time,
                            vector<int> &low_time, vector<int> &bridges,
                            int &time) const;

public:
    explicit Graph(int V_) : V(V_), vertices(V_) {}

    void AddEdge(int from, int to, int edge_n);

    void GetBridges(vector<int> &bridges) const;
};


// Среднее время работы: O(1)
void Graph::AddEdge(int from, int to, int edge_n) {
    // Для вершины from
    if (vertices[from].find(to) == vertices[from].end()) {
        // Ребро встречается в первый раз
        vertices[from][to] = pair{1, edge_n};
    } else {
        // Ребро встречается повторно
        vertices[from][to].first++;
        vertices[from][to].second = edge_n;
    }

    // Для вершины to
    if (vertices[to].find(from) == vertices[to].end()) {
        // Ребро встречается в первый раз
        vertices[to][from] = pair{1, edge_n};
    } else {
        // Ребро встречается повторно
        vertices[to][from].first++;
        vertices[to][from].second = edge_n;
    }
}


void Graph::BridgeDFSRecursive(int cur, vector<bool> &visited,
                               vector<int> &parent, vector<int> &disc_time,
                               vector<int> &low_time, vector<int> &bridges,
                               int &time) const {
    // Факт и время посещения вершины
    visited[cur] = true;
    disc_time[cur] = ++time;
    low_time[cur] = time;

    // Для всех смежных вершин
    for (auto it = vertices[cur].begin(); it != vertices[cur].end(); it++) {
        int adj_vertex = it->first;

        if (!visited[adj_vertex]) {
            // Посещение вершины adj_vertex в первый раз
            parent[adj_vertex] = cur;
            BridgeDFSRecursive(adj_vertex, visited, parent, disc_time, low_time,
                               bridges, time);
            low_time[cur] = min(low_time[cur], low_time[adj_vertex]);

            // edge_count - количество повторений текущего ребра
            // edge_n - номер, с которым текущее ребро пришло в последний раз
            // Мостом может быть только ребро, входящее в граф один раз, поэтому
            // edge_n для моста - не только последний номер, но и единственный
            // за все время
            int edge_count = it->second.first;
            int edge_n = it->second.second;

            if ((low_time[adj_vertex] > disc_time[cur]) && (edge_count == 1)) {
                // Время посещения cur меньше, чем время посещения самой ранней
                // связанной с adj_vertex не через cur вершины
                // Значит, adj_vertex связана только с cur, и ребро
                // cur-adj_vertex - мост
                bridges.push_back(edge_n);
            }
        } else if (adj_vertex != parent[cur]) {
            // Посещение вершины adj_vertex не в первый раз, и она - не родитель
            // cur
            low_time[cur] = min(low_time[cur], disc_time[adj_vertex]);
        }
    }
}


struct GraphSnapshot {
    int cur;
    map_iter it;
    bool is_first_stage;

    GraphSnapshot(int cur_, map_iter it_, bool is_first_stage_) :
            cur(cur_),
            it(it_),
            is_first_stage(is_first_stage_) {}
};


void Graph::BridgeDFSIterative(int cur, vector<bool> &visited,
                               vector<int> &parent, vector<int> &disc_time,
                               vector<int> &low_time, vector<int> &bridges,
                               int &time) const {
    std::stack<GraphSnapshot> stack;
    stack.emplace(cur, vertices[cur].begin(), true);

    while (!stack.empty()) {
        GraphSnapshot current_snapshot = stack.top();
        stack.pop();

        if (current_snapshot.is_first_stage) {
            // До рекурсивного вызова
            cur = current_snapshot.cur;
            auto it = current_snapshot.it;

            // Факт и время посещения вершины
            visited[cur] = true;
            disc_time[cur] = ++time;
            low_time[cur] = time;

            // Для всех смежных вершин
            for (; it != vertices[cur].end(); it++) {
                int adj_vertex = it->first;

                if (!visited[adj_vertex]) {
                    // Посещение вершины adj_vertex в первый раз
                    parent[adj_vertex] = cur;

                    // Добавление на стек текущего состояния для продолжения
                    // после возвращения из рекурсивного вызова
                    stack.emplace(cur, it, false);

                    // Добавление на стек нового состояния для следующего
                    // рекурсивного вызова
                    stack.emplace(adj_vertex, vertices[adj_vertex].begin(),
                                  true);

                } else if (adj_vertex != parent[cur]) {
                    // Посещение вершины adj_vertex не в первый раз, и она - не
                    // родитель cur
                    low_time[cur] = min(low_time[cur], disc_time[adj_vertex]);
                }
            }
        } else {
            // После рекурсивного вызова
            cur = current_snapshot.cur;
            int adj_vertex = current_snapshot.it->first;

            low_time[cur] = min(low_time[cur], low_time[adj_vertex]);

            // edge_count - количество повторений текущего ребра
            // edge_n - номер, с которым текущее ребро пришло в последний раз
            // Мостом может быть только ребро, входящее в граф один раз, поэтому
            // edge_n для моста - не только последний номер, но и единственный
            // за все время
            int edge_count = current_snapshot.it->second.first;
            int edge_n = current_snapshot.it->second.second;

            if ((low_time[adj_vertex] > disc_time[cur]) && (edge_count == 1)) {
                // Время посещения cur меньше, чем время посещения самой ранней
                // связанной с adj_vertex не через cur вершины
                // Значит, adj_vertex связана только с cur, и ребро
                // cur-adj_vertex - мост
                bridges.push_back(edge_n);
            }
        }
    }
}


// Время работы: O(V + E),
// где V - количество вершин графа, E - количество ребер графа
void Graph::GetBridges(vector<int> &bridges) const {
    // Для каждой вершины в векторах содержится информация:
    // visited - флаг посещения (была ли вершина посещена хоть раз)
    // parent - родительская вершина во время DFS
    // disc_time - время первого посещения
    // low_time - время посещения самой ранней связанной не через родителя
    // вершины

    vector<bool> visited(V, false);
    vector<int> parent(V, -1);
    vector<int> disc_time(V);
    vector<int> low_time(V);

    // time - счетчик времени (количество итераций)
    int time = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
//            BridgeDFSIterative(i, visited, parent, disc_time, low_time,
//                               bridges, time);
            BridgeDFSRecursive(i, visited, parent, disc_time, low_time,
                               bridges, time);
        }
    }
}


int main() {
//    const string file_folder = "/Users/n.guryev/CLionProjects/Hometask_12/";
    const string file_folder;
    const string in_file = file_folder + "bridges.in";
    const string out_file = file_folder + "bridges.out";

    IInputStream input(in_file);
    IOutputStream output(out_file);

    // Чтение количества вершин и количества ребер
    pair VE = input.Read();
    int V = VE.first;
    int E = VE.second;
    Graph graph(V);

    // Чтение ребер
    for (int i = 0; i < E; i++) {
        pair from_to = input.Read();

        // Уменьшение на единицу, т.к. номера ребер на входе - от 1 до V
        int from = from_to.first - 1;
        int to = from_to.second - 1;
        graph.AddEdge(from, to, i);
    }

    // Нахождение мостов
    vector<int> bridges;
    graph.GetBridges(bridges);

    // Запись количества мостов
    output.Write(bridges.size());
    std::sort(bridges.begin(), bridges.end());

    // Запись номеров мостов
    // Увеличение на единицу, т.к. номера ребер на входе - от 1 до V
    for (int i : bridges) {
        output.Write("\n");
        output.Write(i + 1);
    }

    return 0;
}
