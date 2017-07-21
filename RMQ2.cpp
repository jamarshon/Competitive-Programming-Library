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

// split arr into sqrt(N) DP: process O(n) query O(sqrt(N))
function<int(int, int)> RMQ2(const vector<int>& arr) {
    assert(!arr.empty());

    int N = arr.size();
    double dbl_sqrt_N = sqrt(N);
    int sqrt_N = dbl_sqrt_N;

    vector<int> M(ceil((double)N/sqrt_N));

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

    function<int(int,int)> test = [](int i, int j){ return 0; };
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
        RMQTestEvaluate(i, A, RMQ2(A));
    }
}

} // namespace RMQTest


int main() {
    RMQTest::RMQTest();
    return 0;
}