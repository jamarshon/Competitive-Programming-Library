#include <iostream>
#include <vector>
#include <unordered_map>
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
};

int main() {
    return 0;
}