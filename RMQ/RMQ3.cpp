#include <iostream>
#include <functional>
#include <cassert>
#include <climits>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

// sparse table: process O(nlogn) query O(1)
function<int(int, int)> RMQ3(const vector<int>& arr) {
    assert(!arr.empty());

    int N = arr.size();
    int log_N = log2(N);

    // M[i][j] represents the minimum of index i to index i + 2^j
    vector<vector<int>> M(N, vector<int>(log_N + 1));

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
        RMQTestEvaluate(i, A, RMQ3(A));
    }
}

} // namespace RMQTest


int main() {
    RMQTest::RMQTest();
    return 0;
}