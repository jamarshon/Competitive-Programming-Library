#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <stack>
#include <queue>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <algorithm>
#include <functional>
 
using namespace std;

#define pii pair<int,int>
#define vt vector
#define uos unordered_set
#define uom unordered_map

typedef long long ll;

class Graph {
    int V;
    list<int> *adj;
public :
    Graph(int V);
    void addEdge(int v,int w);
    bool isCyclic();
};

bool util(const list<int> * adj, int v, unordered_set<int>& visited, int parent) {
    for(const auto& n: adj[v]) {
        if(n == parent) continue;
        if(visited.count(n)) return true;
        visited.insert(n);
        if(util(adj, n, visited, v)) return true;
    }
    return false;
}

bool Graph :: isCyclic() {
    unordered_set<int> visited;
    for(int i = 0; i < V; i++) {
        if(visited.count(i)) continue;
        visited.insert(i);
        if(util(adj, i, visited, -1)) return true;
    }
    
    return false;
}

int main() {
    return 0;
}