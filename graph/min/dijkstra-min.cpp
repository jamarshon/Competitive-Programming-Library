#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <set>
#include <algorithm>

using namespace std;

// edge-min.cpp
template<typename T> 
struct Edge {T to; int weight; Edge(T _to, int _weight): to(_to), weight(_weight) {} 
    bool operator<(const Edge<T>& rhs) const {return (weight == rhs.weight) ? (to < rhs.to) : (weight < rhs.weight); }
};

template<typename T>
// graph-min.cpp
class Graph {unordered_map<T, vector<Edge<T>>> vertex_to_edges_; bool directed_; public: Graph(const bool& directed = false) {directed_ = directed; } void AddVertex(const T& id) {assert(vertex_to_edges_.find(id) == vertex_to_edges_.end() && "Vertex already exists"); vertex_to_edges_[id] = vector<Edge<T>>{}; } void AddEdge(const T& from, const T& to, const int weight = 0) {auto from_edges_it = vertex_to_edges_.find(from); auto to_edges_it = vertex_to_edges_.find(to); assert((from_edges_it != vertex_to_edges_.end() && to_edges_it != vertex_to_edges_.end()) && "Edge already exists"); (from_edges_it -> second).emplace_back(to, weight); if(!directed_) {(to_edges_it -> second).emplace_back(from, weight); } }
// dijkstra-min.cpp
    // Dijkstra's algorithm Time O(ElogV) Space O(E + V)
    pair<unordered_map<T, int>, unordered_map<T, T>> GetShortestPath(const T& source) {assert(vertex_to_edges_.find(source) != vertex_to_edges_.end() && "Source vertex not found"); unordered_map<T, T> parent; set<Edge<T>> edges_to_process; unordered_map<T, int> min_distance; for(auto v_e: vertex_to_edges_) {T vertex = v_e.first; if(vertex == source) continue; edges_to_process.emplace(vertex, INT_MAX); min_distance[vertex] = INT_MAX; } edges_to_process.emplace(source, 0); min_distance[source] = 0; parent[source] = source; while(!edges_to_process.empty()) {Edge<T> min_edge = *edges_to_process.begin(); edges_to_process.erase(edges_to_process.begin()); T current_vertex = min_edge.to; int current_weight = min_edge.weight; if(current_weight == INT_MAX) break; vector<Edge<T>> neighbors = vertex_to_edges_[current_vertex]; for(Edge<T> neighbor: neighbors) {auto edge_set_it = edges_to_process.find({neighbor.to, min_distance[neighbor.to]}); if(edge_set_it == edges_to_process.end()) continue; if(current_weight + neighbor.weight < min_distance[neighbor.to]) {min_distance[neighbor.to] = current_weight + neighbor.weight; parent[neighbor.to] = current_vertex; edges_to_process.erase(edge_set_it); edges_to_process.emplace(neighbor.to, min_distance[neighbor.to]); } } } return {min_distance, parent}; }

    vector<T> RecreatePath(const T source, const T dest, unordered_map<T, T>& parent) {vector<T> res; for(T i = dest; i != source; i = parent[i]) {res.push_back(i); } res.push_back(source); reverse(res.begin(), res.end()); return res; }

    pair<unordered_map<T, int>, unordered_map<T, vector<T>>> GetAllShortestPaths(const T& source) {assert(vertex_to_edges_.find(source) != vertex_to_edges_.end() && "Source vertex not found"); unordered_map<T, vector<T>> parents; set<Edge<T>> edges_to_process; unordered_map<T, int> min_distance; for(auto v_e: vertex_to_edges_) {T vertex = v_e.first; if(vertex == source) continue; edges_to_process.emplace(vertex, INT_MAX); min_distance[vertex] = INT_MAX; } edges_to_process.emplace(source, 0); min_distance[source] = 0; parents[source] = {source}; while(!edges_to_process.empty()) {Edge<T> min_edge = *edges_to_process.begin(); edges_to_process.erase(edges_to_process.begin()); T current_vertex = min_edge.to; int current_weight = min_edge.weight; if(current_weight == INT_MAX) break; vector<Edge<T>> neighbors = vertex_to_edges_[current_vertex]; for(Edge<T> neighbor: neighbors) {auto edge_set_it = edges_to_process.find({neighbor.to, min_distance[neighbor.to]}); if(edge_set_it == edges_to_process.end()) continue; if(current_weight + neighbor.weight == min_distance[neighbor.to]) {parents[neighbor.to].push_back(current_vertex); } else if(current_weight + neighbor.weight < min_distance[neighbor.to]) {min_distance[neighbor.to] = current_weight + neighbor.weight; parents[neighbor.to] = {current_vertex}; edges_to_process.erase(edge_set_it); edges_to_process.emplace(neighbor.to, min_distance[neighbor.to]); } } } return {min_distance, parents}; }

    void RecreateDFS(const T source, const T curr, unordered_map<T, vector<T>>& parents, vector<T>& temp, vector<vector<T>>& res) {if(curr == source) {vector<int> temp_rev(temp.rbegin(), temp.rend()); res.push_back(temp_rev); return; } for(auto from: parents[curr]) {temp.push_back(from); RecreateDFS(source, from, parents, temp, res); temp.pop_back(); } }

    vector<vector<T>> RecreateAllPaths(const T source, const T dest, unordered_map<T, vector<T>>& parents) {vector<vector<T>> res; vector<T> temp{dest}; RecreateDFS(source, dest, parents, temp, res); return res; }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace GraphTest {

// dijkstra-min.cpp
void TestGetShortestPath1() {Graph<char> g; vector<char> v{'A', 'B', 'C', 'D', 'E', 'F'}; for(auto c: v) g.AddVertex(c); g.AddEdge('A', 'B', 5); g.AddEdge('B', 'C', 2); g.AddEdge('C', 'D', 3); g.AddEdge('A', 'D', 9); g.AddEdge('A', 'E', 2); g.AddEdge('D', 'F', 2); g.AddEdge('E', 'F', 3); unordered_map<char, int> distance; unordered_map<char, char> parent; tie(distance, parent) = g.GetShortestPath('A'); vector<char> output_path = g.RecreatePath('A', 'F', parent); unordered_map<char, int> expected_distance{{'A',0}, {'B',5}, {'C',7}, {'D',7}, {'E',2}, {'F',5}, }; unordered_map<char, char> expected_parent{{'C','B'}, {'F','E'}, {'E','A'}, {'D','F'}, {'B','A'}, {'A','A'}, }; vector<char> expected_path{'A', 'E', 'F'}; cout << "Run distance test" << endl; assert(expected_distance.size() == distance.size()); for(auto d: distance) {assert(expected_distance[d.first] == d.second); } cout << "Pass distance test" << endl; cout << "Run parent test" << endl; assert(expected_parent.size() == expected_parent.size()); for(auto p: parent) {assert(expected_parent[p.first] == p.second); } cout << "Pass parent test" << endl; cout << "Run path test" << endl; int N = output_path.size(); assert(N == expected_path.size()); for(int i = 0; i < N; i++) {assert(expected_path[i] == output_path[i]); } cout << "Pass path test" << endl; }

void TestGetShortestPath2() {Graph<int> g; vector<int> v{0,1,2,3,4,5,6,7,8}; for(auto d: v) g.AddVertex(d); g.AddEdge(0, 1, 4); g.AddEdge(0, 7, 8); g.AddEdge(1, 2, 8); g.AddEdge(1, 7, 11); g.AddEdge(2, 3, 7); g.AddEdge(2, 8, 3); g.AddEdge(2, 5, 4); g.AddEdge(3, 4, 9); g.AddEdge(3, 5, 14); g.AddEdge(4, 5, 10); g.AddEdge(5, 6, 2); g.AddEdge(6, 7, 1); g.AddEdge(6, 8, 6); g.AddEdge(7, 8, 7); unordered_map<int, int> distance; unordered_map<int, int> parent; tie(distance, parent) = g.GetShortestPath(0); unordered_map<int, int> expected_distance{{0,0}, {1,4}, {2,12}, {3,19}, {4,21}, {5,11}, {6,9}, {7,8}, {8,15}, }; unordered_map<int, int> expected_parent{{4,5}, {3,2}, {5,6}, {8,7}, {6,7}, {2,1}, {7,0}, {1,0}, {0,0}, }; vector<vector<int>> expected_paths {{0, 7, 8}, {0, 7, 6, 8}, {0, 1, 2, 8}, }; cout << "Run distance test" << endl; assert(expected_distance.size() == distance.size()); for(auto d: distance) {assert(expected_distance[d.first] == d.second); } cout << "Pass distance test" << endl; cout << "Run parent test" << endl; assert(expected_parent.size() == expected_parent.size()); for(auto p: parent) {assert(expected_parent[p.first] == p.second); } cout << "Pass parent test" << endl; unordered_map<int, vector<int>> parents; tie(distance, parents) = g.GetAllShortestPaths(0); vector<vector<int>> output_paths = g.RecreateAllPaths(0, 8, parents); cout << "Run all shortest distance test" << endl; assert(expected_distance.size() == distance.size()); for(auto d: distance) {assert(expected_distance[d.first] == d.second); } cout << "Pass all shortest distance test" << endl; cout << "Run all shortest path test" << endl; int N = output_paths.size(); assert(N == expected_paths.size()); for(int i = 0; i < N; i++) {int M = output_paths[i].size(); assert(M == expected_paths[i].size()); for(int j = 0; j < M; j++) {assert(output_paths[i][j] == expected_paths[i][j]); } } cout << "Pass all shortest path test" << endl; }
} // namespace GraphTest

int main() {
    GraphTest::TestGetShortestPath1();
    GraphTest::TestGetShortestPath2();
    return 0;
}