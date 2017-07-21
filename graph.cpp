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

int main() {
    // TestGetShortestPath1();
    // TestGetShortestPath2();
    // TestTopologicalSort();
    return 0;
}