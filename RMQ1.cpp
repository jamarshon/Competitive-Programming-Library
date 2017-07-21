#include <iostream>
#include <functional>
#include <cassert>
#include <climits>
#include <cmath>
#include <vector>

using namespace std;

// Simple DP: process O(n^2) query O(1)
function<int(int, int)> RMQ1(const vector<int>& arr) {
    assert(!arr.empty());

    int N = arr.size();

    vector<vector<int>> M(N, vector<int>(N));

    // one element 
    for(int i = 0; i < N; i++) {
        M[i][i] = i;
    }

    // if the previous minimum from i to j - 1 < current than take current element index
    // else take previous minimimum index
    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            if(arr[M[i][j-1]] < arr[j]) {
                M[i][j] = M[i][j-1];
            } else {
                M[i][j] = j;
            }
        }
    }

    function<int(int, int)> query = [M](const int& i, const int& j){
        return M[i][j];
    };

    return query;
}

void RMQTest(const vector<int>& arr, function<int(int, int)> f) {
    int N = arr.size();
    for(int i = 0; i < N; i++) {
        for(int j = i; j < N; j++) {
            int test = f(i, j);
            int expect = INT_MAX;
            for(int k = i; k <= j; k++) {
                expect = min(expect, arr[k]);
            }
            // cout << arr[test] << ' ' << expect << endl;
            assert(arr[test] == expect);
        }
    }
    printf("RMQTest complete\n");
}

int main() {
    vector<int> arr{2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
    RMQTest(arr, RMQ1(arr));
    return 0;
}