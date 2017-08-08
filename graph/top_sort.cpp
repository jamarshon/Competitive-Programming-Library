#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cassert>

using namespace std;

// edge-min.cpp
template<typename T> 
struct Edge {T to; int weight; Edge(T _to, int _weight): to(_to), weight(_weight) {} 
    bool operator<(const Edge<T>& rhs) const {return (weight == rhs.weight) ? (to < rhs.to) : (weight < rhs.weight); }
};

// graph-min.cpp
template<typename T>
class Graph {unordered_map<T, vector<Edge<T>>> vertex_to_edges_; bool directed_; public: Graph(const bool& directed = false) {directed_ = directed; } void AddVertex(const T& id) {assert(vertex_to_edges_.find(id) == vertex_to_edges_.end() && "Vertex already exists"); vertex_to_edges_[id] = vector<Edge<T>>{}; } void AddEdge(const T& from, const T& to, const int weight = 0) {auto from_edges_it = vertex_to_edges_.find(from); auto to_edges_it = vertex_to_edges_.find(to); assert((from_edges_it != vertex_to_edges_.end() && to_edges_it != vertex_to_edges_.end()) && "Edge already exists"); (from_edges_it -> second).emplace_back(to, weight); if(!directed_) {(to_edges_it -> second).emplace_back(from, weight); } }
    bool TopologicalSortUtil(const T prev, const T vertex, unordered_set<T>& visited, 
        unordered_set<T>& expanding, stack<T>& st) {

        if(expanding.count(vertex)) return true;

        expanding.insert(vertex);
        
        for(auto neighbor: vertex_to_edges_[vertex]) {
            if(!directed_ && neighbor.to == prev) continue;
            if(visited.count(neighbor.to)) continue;
            bool has_cycle = TopologicalSortUtil(vertex, neighbor.to, visited, expanding, st);
            if(has_cycle) return true;
        }

        visited.insert(vertex);
        expanding.erase(vertex);

        st.push(vertex);
        return false;
    }

    vector<T> TopologicalSort() {
        unordered_set<T> visited;
        unordered_set<T> expanding;
        stack<T> st;

        bool has_cycle = false;
        for(auto ve: vertex_to_edges_) {
            T vertex = ve.first;
            if(visited.count(vertex)) continue;
            T prev;
            has_cycle = TopologicalSortUtil(prev, vertex, visited, expanding, st);
            if(has_cycle) break;
        }

        vector<T> res;
        if(!has_cycle) {
            while(!st.empty()) {
                res.push_back(st.top());
                st.pop();
            }
        }

        return res;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace GraphTest {
void TestTopologicalSort1() {
    // directed no cycle
    Graph<char> g(true);
    vector<char> v{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    for(auto c: v) g.AddVertex(c);

    g.AddEdge('A', 'C');
    g.AddEdge('B', 'C');
    g.AddEdge('B', 'D');
    g.AddEdge('C', 'E');
    g.AddEdge('D', 'F');
    g.AddEdge('E', 'F');
    g.AddEdge('F', 'G');

    vector<char> output;
    output = g.TopologicalSort();

    int output_len = output.size();

    cout << "Run topological sort test" << endl;
    assert(v.size() == output_len);
    for(int i = 0; i < output_len; i++) {
        assert(v[i] == output[i]);
    }
    cout << "Pass topological sort test" << endl;
}

void TestTopologicalSort2() {
    // directed with cycle
    Graph<int> g(true);
    vector<int> v{0,1,2,3,4};
    for(auto n: v) g.AddVertex(n);

    g.AddEdge(0, 1);
    g.AddEdge(2, 0);
    g.AddEdge(0, 3);
    g.AddEdge(1, 2);
    g.AddEdge(3, 4);

    vector<int> output = g.TopologicalSort();

    cout << "Run direct with cycle test" << endl;
    assert(output.empty());
    cout << "Pass direct with cycle test" << endl;
}

void TestTopologicalSort3() {
    // undirected with cycle
    Graph<int> g;
    vector<int> v{0,1,2,3,4};
    for(auto n: v) g.AddVertex(n);

    g.AddEdge(0, 1);
    g.AddEdge(2, 0);
    g.AddEdge(0, 3);
    g.AddEdge(1, 2);
    g.AddEdge(3, 4);

    vector<int> output = g.TopologicalSort();
    
    cout << "Run undirected with cycle test" << endl;
    assert(output.empty());
    cout << "Pass undirected with cycle test" << endl;
}

void TestTopologicalSort4() {
    // undirected with no cycle
    Graph<int> g;
    vector<int> v{0,1,2,3,4};
    for(auto n: v) g.AddVertex(n);

    g.AddEdge(0, 1);
    g.AddEdge(0, 2);
    g.AddEdge(0, 3);
    g.AddEdge(3, 4);

    vector<int> output = g.TopologicalSort();
    vector<int> expected{4,3,0,2,1};

    int N = output.size();
    cout << "Run undirected without cycle test" << endl;
    assert(N != 0);
    for(int i = 0; i < N; i++) {
        assert(output[i] == expected[i]);
    }
    cout << "Pass cycle test" << endl;
}
} // namespace GraphTest

int main() {
    GraphTest::TestTopologicalSort1();
    GraphTest::TestTopologicalSort2();
    GraphTest::TestTopologicalSort3();
    GraphTest::TestTopologicalSort4();
    return 0;
}