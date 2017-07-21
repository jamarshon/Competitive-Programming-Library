#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <climits>
#include <cassert>
#include <stack>

using namespace std;

template<typename T>
struct Edge {
    T to;
    int weight;
    Edge(T _to, int _weight): to(_to), weight(_weight) {}
    bool operator < (const Edge& rhs) const {
        return (weight == rhs.weight) ? (to < rhs.to) : (weight < rhs.weight);
    }
};

template<typename T>
class Graph {
    // key is from, value is all the connecting edges that has an origin from the key
    unordered_map<T, vector<Edge<T>>> vertex_to_edges_;
    bool directed_;
public:
    Graph(const bool& directed = false) {
        directed_ = directed;
    }

    void AddVertex(const T& id) {
        assert(vertex_to_edges_.find(id) == vertex_to_edges_.end() && "Vertex already exists");
        vertex_to_edges_[id] = vector<Edge<T>>{};
    }

    void AddEdge(const T& from, const T& to, const int& weight) {
        typename unordered_map<T, vector<Edge<T>>>::iterator from_edges = vertex_to_edges_.find(from);
        typename unordered_map<T, vector<Edge<T>>>::iterator to_edges = vertex_to_edges_.find(to);
        assert((from_edges != vertex_to_edges_.end() && to_edges != vertex_to_edges_.end()) && "Edge already exists");
        (from_edges -> second).emplace_back(to, weight);
        if(!directed_) {
            (to_edges -> second).emplace_back(from, weight);
        }
    }

    // Dijkstra's algorithm Time O(ElogV) Space O(E + V)
    pair<unordered_map<T, int>, unordered_map<T, T>> GetShortestPath(const T& source) {
        assert(vertex_to_edges_.find(source) != vertex_to_edges_.end() && "Source vertex not found");
        unordered_map<T, T> parent;
        set<Edge<T>> edges_to_process;
        unordered_map<T, int> min_distance;
        
        for(auto v_e: vertex_to_edges_) {
            T vertex = v_e.first;
            if(vertex == source) continue;
            edges_to_process.emplace(vertex, INT_MAX);
            min_distance[vertex] = INT_MAX;
        }
        
        edges_to_process.emplace(source, 0);
        min_distance[source] = 0;
        parent[source] = source;

        while(!edges_to_process.empty()) {
            // Minimum weight edge
            Edge<T> min_edge = *edges_to_process.begin();
            edges_to_process.erase(edges_to_process.begin());

            T current_vertex = min_edge.to;
            int current_weight = min_edge.weight;
            
            vector<Edge<T>> neighbors = vertex_to_edges_[current_vertex];
            for(Edge<T> neighbor: neighbors) {
                typename set<Edge<T>>::iterator edge_set_it = edges_to_process.find({neighbor.to, min_distance[neighbor.to]});
                // done processing already
                if(edge_set_it == edges_to_process.end()) continue;

                // found a smaller distance
                if(current_weight + neighbor.weight < min_distance[neighbor.to]) {
                    min_distance[neighbor.to] = current_weight + neighbor.weight;
                    parent[neighbor.to] = current_vertex;

                    edges_to_process.erase(edge_set_it);
                    edges_to_process.emplace(neighbor.to, min_distance[neighbor.to]);
                }
            }
        }

        return {min_distance, parent};
    }

    void TopologicalSortUtil(unordered_set<T>& visited, stack<T>& topological_order, T vertex) {
        // mark as visited
        visited.insert(vertex);
        vector<Edge<T>> neighbors = vertex_to_edges_[vertex];

        // visit all the children of this vertex
        for(Edge<T> edge: neighbors) {
            if(visited.find(edge.to) == visited.end()) {
                TopologicalSortUtil(visited, topological_order, vertex);
            }
        }

        topological_order.push(vertex);
    }

    vector<T> TopologicalSort() {
        stack<T> topological_order;
        unordered_set<T> visited;

        for(auto v_to_e: vertex_to_edges_) {
            T vertex = v_to_e.first;
            if(visited.find(vertex) == visited.end()) {
                TopologicalSortUtil(visited, topological_order, vertex);
            }
        }

        vector<T> order_arr;
        order_arr.reserve(topological_order.size());
        
        while(!topological_order.empty()) {
            order_arr.push_back(topological_order.top());
            topological_order.pop();
        }

        return order_arr;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace GraphTest {

void TestGetShortestPath1() {
    Graph<char> g;
    vector<char> v{'A', 'B', 'C', 'D', 'E', 'F'};
    for(auto c: v) g.AddVertex(c);
    g.AddEdge('A', 'B', 5);
    g.AddEdge('B', 'C', 2);
    g.AddEdge('C', 'D', 3);
    g.AddEdge('A', 'D', 9);
    g.AddEdge('A', 'E', 2);
    g.AddEdge('D', 'F', 2);
    g.AddEdge('E', 'F', 3);

    pair<unordered_map<char, int>, unordered_map<char, char>> output;
    output = g.GetShortestPath('A');

    unordered_map<char, int> distance;
    unordered_map<char, char> parent;
    tie(distance, parent) = output;

    unordered_map<char, int> expected_distance{
        {'A',0},
        {'B',5},
        {'C',7},
        {'D',7},
        {'E',2},
        {'F',5},
    };

    unordered_map<char, char> expected_parent{
        {'C','B'},
        {'F','E'},
        {'E','A'},
        {'D','F'},
        {'B','A'},
        {'A','A'},
    };

    cout << "Run distance test" << endl;
    assert(expected_distance.size() == distance.size());
    for(auto d: distance) {
        assert(expected_distance[d.first] == d.second);
    }
    cout << "Pass distance test" << endl;

    cout << "Run parent test" << endl;
    assert(expected_parent.size() == expected_parent.size());
    for(auto p: parent) {
        assert(expected_parent[p.first] == p.second);
    }
    cout << "Pass parent test" << endl;
}

void TestGetShortestPath2() {
    Graph<int> g;
    vector<int> v{0,1,2,3,4,5,6,7,8};
    for(auto d: v) g.AddVertex(d);
    g.AddEdge(0, 1, 4);
    g.AddEdge(0, 7, 8);
    g.AddEdge(1, 2, 8);
    g.AddEdge(1, 7, 11);
    g.AddEdge(2, 3, 7);
    g.AddEdge(2, 8, 2);
    g.AddEdge(2, 5, 4);
    g.AddEdge(3, 4, 9);
    g.AddEdge(3, 5, 14);
    g.AddEdge(4, 5, 10);
    g.AddEdge(5, 6, 2);
    g.AddEdge(6, 7, 1);
    g.AddEdge(6, 8, 6);
    g.AddEdge(7, 8, 7);

    pair<unordered_map<int, int>, unordered_map<int, int>> output;
    output = g.GetShortestPath(0);

    unordered_map<int, int> distance;
    unordered_map<int, int> parent;
    tie(distance, parent) = output;

    unordered_map<int, int> expected_distance{
        {0,0},
        {1,4},
        {2,12},
        {3,19},
        {4,21},
        {5,11},
        {6,9},
        {7,8},
        {8,14},
    };

    unordered_map<int, int> expected_parent{
        {4,5},
        {3,2},
        {5,6},
        {8,2},
        {6,7},
        {2,1},
        {7,0},
        {1,0},
        {0,0},
    };

    cout << "Run distance test" << endl;
    assert(expected_distance.size() == distance.size());
    for(auto d: distance) {
        assert(expected_distance[d.first] == d.second);
    }
    cout << "Pass distance test" << endl;

    cout << "Run parent test" << endl;
    assert(expected_parent.size() == expected_parent.size());
    for(auto p: parent) {
        cout << p.first << ' ' << p.second << endl;
        assert(expected_parent[p.first] == p.second);
    }
    cout << "Pass parent test" << endl;
}
void TestTopologicalSort() {
    Graph<char> g(true);
    vector<char> v{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    for(auto c: v) g.AddVertex(c);

    g.AddEdge('A', 'C', 0);
    g.AddEdge('B', 'C', 0);
    g.AddEdge('B', 'D', 0);
    g.AddEdge('C', 'E', 0);
    g.AddEdge('D', 'F', 0);
    g.AddEdge('E', 'F', 0);
    g.AddEdge('F', 'G', 0);

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

} // namespace GraphTest


int main() {
    GraphTest::TestGetShortestPath1();
    GraphTest::TestGetShortestPath2();
    GraphTest::TestTopologicalSort();
    return 0;
}