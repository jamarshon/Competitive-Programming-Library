#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
 
using namespace std;

// longest increasing subsequence using DP O(n^2) dp[i] represents lis that ends at this index
vector<int> LIS(const vector<int>& v) {
    int N = v.size();
    if(N == 0) return {};

    vector<int> dp(N, 1), P(N, -1);

    int lis_ind = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < i; j++) {
            if(v[j] < v[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                P[i] = j;
            }
        }
        if(dp[lis_ind] < dp[i]) lis_ind = i;
    }
 
    vector<int> lis_seq(dp[lis_ind]);

    int index = lis_seq.size();
    for(int i = lis_ind; i >= 0; i = P[i])
        lis_seq[--index] = v[i];

    return lis_seq;
}
 
// O(nlogn) where dp[i] represents the smallest end of a sequence of length i
vector<int> LIS2(const vector<int>& v) {
    int N = v.size();

    vector<int> dp(N, -1), P(N, -1);

    int dp_len = 0;
    for(int i = 0; i < N; i++) {
        auto it = lower_bound(dp.begin(), dp.begin() + dp_len, v[i], [&v](const int dp_el, const int val){
            return v[dp_el] < val;
        });
 
        int it_index = it - dp.begin();
        dp[it_index] = i;
        if(it_index > 0) P[i] = dp[it_index - 1];
        if(it_index == dp_len) dp_len++;
    }
 
    vector<int> lis_seq(dp_len);

    int index = lis_seq.size();
    for(int i = dp[dp_len - 1]; i >= 0; i = P[i])
        lis_seq[--index] = v[i];

    return lis_seq;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace LISTest {

void LISTestEvaluate(const int i, const string type, const vector<int>& input, 
    const vector<int>& output, const int expected_len) {
    assert(output.size() == expected_len);

    int index = 0;

    for(int i = 0; i < input.size() && index < output.size(); i++) {
        if(input[i] == output[index]) {
            index++;
        }
    }

    assert(index == output.size());
    printf("PASS #%d: type %s \n", i, type.c_str());
}

void LISTest() {
    vector<vector<int>> inputs {
        {3, 10, 2, 1, 20}, // 3
        {3, 2}, // 1
        {50, 3, 10, 7, 40, 80}, // 4
        {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15}, // 6
    };

    vector<int> expected_lens{3,1,4,6};

    for(int i = 0; i < inputs.size(); i++) {
        LISTestEvaluate(i, "n^2", inputs[i], LIS(inputs[i]), expected_lens[i]);
        LISTestEvaluate(i, "nlogn", inputs[i], LIS2(inputs[i]), expected_lens[i]);
    }
}

} // namespace LISTest

int main() {
    LISTest::LISTest();   
    return 0;
}