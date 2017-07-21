#include <iostream>
#include <functional>
#include <cassert>
#include <climits>
#include <cmath>
#include <vector>

using namespace std;

// split arr into sqrt(N) DP: process O(n) query O(sqrt(N))
function<int(int, int)> RMQ2(const vector<int>& arr) {
    assert(!arr.empty());

    int N = arr.size();
    double dbl_sqrt_N = sqrt(N);
    int sqrt_N = dbl_sqrt_N;

    vector<int> M(ceil(dbl_sqrt_N));

    // M holds the minimum for each block of sqrt_N
    for(int i = 0; i < N; i++) {
        if(i % sqrt_N == 0 || arr[i] < arr[M[i/sqrt_N]]) {
            M[i/sqrt_N] = i;
        }
    }

    // iterate from i to j everytime we can take a sqrt_N jump then we do
    function<int(int, int)> query = [arr, M, sqrt_N](const int& i, const int& j){
        int res = i;
        for(int k = i; k <= j;) {
            if(k % sqrt_N == 0 && k + sqrt_N <= j) {
                if(arr[M[k/sqrt_N]] < arr[res]) {
                    res = M[k/sqrt_N];
                }
                k += sqrt_N;
            } else {
                if(arr[k] < arr[res]) { 
                    res = k; 
                }
                k++;
            }
        }

        return res;
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
    RMQTest(arr, RMQ2(arr));
    return 0;
}