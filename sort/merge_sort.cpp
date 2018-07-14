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

void mergeSort(vector<int>& v, vector<int>& sp, int start, int end) {
    if(start >= end) return;

    int mid = start + (end - start)/2;
    mergeSort(v, sp, start, mid);
    mergeSort(v, sp, mid + 1, end);

    int idx = 0;
    int s1 = start, s2 = mid + 1;
    int e1 = mid + 1, e2 = end + 1;
    while(s1 < e1 || s2 < e2) {
        if(s2 >= e2 || (s1 < e1 && v[s1] < v[s2])) {
            sp[idx++] = v[s1];
            s1++;
        } else {
            sp[idx++] = v[s2];
            s2++;
        }
    }

    for(int i = 0; i < idx; i++) v[i + start] = sp[i];
}

int main() {
    vector<int> v{5, 2, 4, 6, 1};
    vector<int> sp(v.size());
    mergeSort(v, sp, 0, v.size() - 1);
    for(auto e: v) cout << e << ' '; cout << endl;
    return 0;
}