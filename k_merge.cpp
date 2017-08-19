#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <queue>
#include <algorithm>
#include <stack>
#include <functional>
#include <unordered_map>
#include <cassert>
#include <bitset>
#include <random>
#include <chrono>
#include <map>
 
using namespace std;
 
struct MergeItem {
    const vector<int>* arr;
    int index;
    MergeItem(const vector<int>* _arr, int _index) : arr(_arr), index(_index) {}
    bool operator>(const MergeItem& rhs) const {
        return arr -> at(index) > rhs.arr -> at(rhs.index);
    }
};
 
vector<int> merge(const vector<vector<int>> v) {
    vector<int> res;
    int K = v.size();
 
    typedef priority_queue<MergeItem, vector<MergeItem>, greater<MergeItem>> MinHeap;
    MinHeap pq;
 
    int res_size = 0;
    for(int i = 0; i < K; i++) {
        if(v[i].empty()) continue;
        pq.emplace(&v[i], 0);
        res_size += v[i].size();
    }
 
    res.reserve(res_size);
 
    while(!pq.empty()) {
        MergeItem top = pq.top();
        pq.pop();
 
        int item = top.arr -> at(top.index++);
        res.push_back(item);
        if(top.index < top.arr -> size()) {
            pq.push(top);
        }
    }
    return res;
}
 
int main() {
    vector<int> v = merge({
        {0,4,5,6,8},
        {1,1,1,2},
        {3,3,9,10}
    });
 
    for(auto e: v) cout << e << ' ';
    return 0;
}