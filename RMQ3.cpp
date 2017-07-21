#include <iostream>
#include <functional>
#include <cassert>
#include <climits>
#include <cmath>
#include <vector>

using namespace std;

// sparse table: process O(nlogn) query O(1)
function<int(int, int)> RMQ3(const vector<int>& arr) {
    assert(!arr.empty());

    int N = arr.size();
    int log_N = ceil(log2(N));

    // M[i][j] represents the minimum of index i to index i + 2^j
    vector<vector<int>> M(N, vector<int>(log_N));

    // one element as jump of 1
    for(int i = 0; i < N; i++) {
        M[i][0] = i;
    }

    // check the minimum of interval i to 2^(j-1) < the minimum of interval i + 2^(j-1) to i + 2^j
    for(int j = 1; (1 << j) <= N; j++) {
        for(int i = 0; i + (1 << (j-1)) < N; i++) {
            if (arr[M[i][j - 1]] < arr[M[i + (1 << (j-1))][j - 1]]) {
                M[i][j] = M[i][j - 1];
            } else {
                M[i][j] = M[i + (1 << (j-1))][j - 1];
            }
        }
    }

    // select two blocks that cover the entire interval [i, j] and find the minimum of them
    function<int(int, int)> query = [arr, M](const int& i, const int& j){
        int k = log2(j - i + 1);

        if(arr[M[i][k]] < arr[M[j - (1 << k) + 1][k]]) {
            return M[i][k];
        } else {
            return M[j - (1 << k) + 1][k];
        }
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
    RMQTest(arr, RMQ3(arr));
    return 0;
}