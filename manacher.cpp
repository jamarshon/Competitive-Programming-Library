#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

string Manacher(string s) {
    const char kNullChar = '\0';
    string str = string(1, kNullChar);

    for(auto c: s) str += string(1, c) + kNullChar;

    string max_str = "";
    int len = str.size();
    int right = 0;
    int center = 0;
    int dp[len];

    fill(dp, dp + sizeof(dp)/sizeof(int), 0);

    for(int i = 1; i < len; i++) {
        int mirr = 2*center - i;

        // i is within right so can take the minimum of the mirror or distance from right
        if(i < right) {
            dp[i] = min(right - i, dp[mirr]);
        }

        // keep expanding around i while it is the same and increment P[i]
        int left_index = i - (1 + dp[i]);
        int right_index = i + (1 + dp[i]);
        while(left_index != 0 && right_index != len - 1 && str[left_index] == str[right_index]) {
            left_index--;
            right_index++;
            dp[i]++;
        }

        int curr_len = right_index - left_index - 1;
        if(curr_len > max_str.size()) {
            max_str = str.substr(left_index + 1, curr_len);
        }

        // i goes beyond current right so it is the new center
        if(i + dp[i] > right) {
            center = i;
            right = i + dp[i];
        }
    }

    string res = "";
    for(auto c: max_str) {
        if(c != kNullChar) res += c;
    }

    return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ManacherTest {

void ManacherTest() {
    vector<pair<string, string>> input_to_output{
        {"forgeeksskeegfor", "geeksskeeg"},
        {"abaaba", "abaaba"},
        {"abababa", "abababa"},
        {"abcbabcbabcba", "abcbabcbabcba"},
        {"abaxabaxabb", "baxabaxab"},
    };

    for(auto input_to_output_pair: input_to_output) {
        assert(Manacher(input_to_output_pair.first) == input_to_output_pair.second);
    }
}
} // namespace ManacherTest

int main() {
    ManacherTest::ManacherTest();
    return 0;
}