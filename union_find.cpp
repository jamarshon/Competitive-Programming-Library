#include <iostream>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <algorithm>
 
using namespace std;
 
template <typename T>
class UnionFind {
    // key is element, value is rank
    unordered_map<T, int> rank_;
    // key is element, value is parent
    unordered_map<T, T> parent_;
public:
    bool IsWithinSet(T e) {
        return parent_.count(e);
    }
 
    void CreateSet(T e) {
        assert(!IsWithinSet(e));
        parent_[e] = e;
        rank_[e] = 0;
    }
 
    // finds the root of e
    T Find(T e) {
        if(parent_[e] != e) {
            // this is not a root (root has parent to be equal itself)
            // so find root and apply path compression along path
            parent_[e] = Find(parent_[e]);
        }
        return parent_[e];
    }
 
    // unions the sets of e1 and e2 if necessary
    // return whether an union took place
    bool Union(T e1, T e2) {
        T e1_root = Find(e1);
        T e2_root = Find(e2);
 
        if(e1_root == e2_root) return false; // same root
 
        // Attach smaller rank tree under root of high rank tree
        // (Union by Rank)
        if(rank_[e1_root] < rank_[e2_root]) {
            parent_[e1_root] = e2_root;
        } else {
            parent_[e2_root] = e1_root;
            if(rank_[e1_root] == rank_[e2_root]) {
                rank_[e1_root]++;
            }
        }
 
        return true;
    }
};
 
// a spanning tree of that graph is a subgraph that is a tree and connects all the vertices together
// with weight less than or equal to the weight of every other spanning tree
vector<tuple<int,int,int>> MinimumSpanningTree(const vector<vector<pair<int,int>>>& g) {
    UnionFind<int> disjoint_sets;
    vector<tuple<int,int,int>> edges;
 
    // initialize each vertex as it's own set
    int number_of_vertices = g.size();
    for(int i = 0; i < number_of_vertices; i++) {
        disjoint_sets.CreateSet(i);
        for(auto e: g[i]) edges.emplace_back(i, e.first, e.second);
    }
   
    // sort by weight
    sort(edges.begin(), edges.end(), [](const tuple<int,int,int>& lhs, const tuple<int,int,int>& rhs){
        return get<2>(lhs) < get<2>(rhs);
    });
 
    int from, to, weight;
 
    vector<tuple<int,int,int>> mst;
    for(auto edge: edges) {
        // spanning tree has n - 1 edges where n is the number_of_vertices
        if(mst.size() == number_of_vertices - 1) break;
        tie(from, to, weight) = edge;
 
        if(disjoint_sets.Union(from, to)) {
            mst.push_back(edge);
        }
    }
 
    // for(auto e: mst) {
    //     tie(from, to, weight) = e;
    //     printf("from=%d to=%d weight=%d\n", from, to, weight);
    // }
    return mst;
}
 
int main() {
    vector<vector<pair<int,int>>> g(9);
    g[0] = {{1,4}, {7,8}};
    g[1] = {{0,4}, {7,11}, {2,8}};
    g[2] = {{1,8}, {3,7}, {5,4}, {8,2}};
    g[3] = {{2,7}, {4,9}, {5,14}};
    g[4] = {{3,9}, {5,10}};
    g[5] = {{2,4}, {3,14}, {4,10}, {6,2}};
    g[6] = {{5,2}, {7,1}, {8,6}};
    g[7] = {{0,8}, {1,11}, {6,1}, {8,7}};
    g[8] = {{2,2}, {6,6}, {7,7}};
 
    vector<tuple<int,int,int>> output = MinimumSpanningTree(g);
    assert(output.size() == 8);
 
    // weight, unordered_map key is min(from,to) value is max(from,to)
    unordered_map<int, unordered_map<int,int>> expected {
        {1, {{6,7}}},
        {2, {{2,8},{5,6}}},
        {4, {{0,1},{2,5}}},
        {7, {{2,3}}},
        {8, {{1,2}}},
        {9, {{3,4}}},
    };
 
    int from, to, weight;
    unordered_map<int, unordered_map<int,int>> output_m;
    for(auto e: output) {
        tie(from, to, weight) = e;
        if(from > to) swap(from, to);
        output_m[weight][from] = to;
    }
 
    assert(expected.size() == output_m.size());

    for(auto kv: expected) {
        assert(output_m.count(kv.first)); // check if weight is in map
 
        unordered_map<int,int> map_expected = kv.second,
        map_output = output_m[kv.first];
 
        assert(map_expected.size() == map_output.size()); // check if same number of edges of weight
 
        for(auto kv_m: map_expected) {
            // check min(from,to) is in output of that weight
            assert(map_output.count(kv_m.first));
            // check if expected[min(from,to)] = map_output[min(from,to)]
            assert(kv_m.second == map_output[kv_m.first]);
        }
    }
    
    return 0;
}