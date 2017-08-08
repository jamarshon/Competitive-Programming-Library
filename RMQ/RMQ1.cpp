#include <iostream>
#include <functional>
#include <cassert>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace RMQTest {

void RMQTestEvaluate(const int i, const vector<int>& arr, function<int(int, int)> f) {
    int N = arr.size();
    for(int i = 0; i < N; i++) {
        for(int j = i; j < N; j++) {
            int test = f(i, j);
            int expect = arr[i];
            for(int k = i; k <= j; k++) {
                expect = min(expect, arr[k]);
            }
            if(arr[test] != expect) cout << arr[test] << ' ' << expect << endl;
            assert(arr[test] == expect);
        }
    }
    printf("PASS #%d\n", i);
}

void RMQTest() {
    vector<int> A;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);
    
    for(int i = 0; i < 1000; i++) {
        int N = uniform_int_distribution<int>(1, 100)(engine);
 
        A.clear();
        A.resize(N);
        uniform_int_distribution<int> dist(-100, 100);
        auto gen = bind(dist, engine);
        generate(A.begin(), A.end(), gen);

        for(auto a: A) cout << a << ','; cout << endl;
        RMQTestEvaluate(i, A, RMQ1(A));
    }
}

} // namespace RMQTest


int main() {
    RMQTest::RMQTest();
    return 0;
}