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

// #define pii pair<int,int>
#define vt vector
#define uos unordered_set
#define uom unordered_map

typedef long long ll;

unordered_map<int, int> dijkstra(unordered_map<int, vector<int>>& adj, int source) {
    typedef pair<int,int> pii;
    unordered_map<int, int> dist;
    for(const auto& u_adj: adj) dist[u_adj.first] = INT_MAX;
    priority_queue<pii, vector<pii>, greater<pii>> pq; // dist, edge and gives smallest dist first. 
    pq.emplace(0, source);
    return dist;
}

int main() {
    return 0;
}