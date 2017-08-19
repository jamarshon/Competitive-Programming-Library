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
#include <climits>
#include <cmath>
#include <algorithm>
#include <functional>
 
using namespace std;

/*
    Given a string, each word separated by a space and maximum line length. The end of the line
    will be remaining space (b). Define messiness as b^2 and the total messiness to be the sum 
    over all lines. Words cannot be split. Find the minimum messiness.
    Example "a b c d" and length 4

    01234
    a_b_c
    d____

    0^2 + 4^2 = 16

    01234
    a_b__
    c_d__
    
    2^2 + 2^2 = 8
*/

/*
    f(j,i) is messiness of a single line consisting of words [j,i]
    M(i) is the minimum messiness of placing first i words
    M(i) = min{j <= i}( f(j,i) + M(j-1))
    we consider all ways of [j,i] where j<=i as the last line plus the minimum
    messiness of everything before the word j so M(j-1) and find the minimum

    Example: "aaa bbb c d ee ff ggggggg" and 11
    [aaa, bbb, c, d, ee, ff, ggggggg]
    0      1   2  3  4  5    6
    M(5) = min(
        f(5,6) + M(4), <- last line is "ff ggggggg" and minimum messiness of "aaa bbb c d ee"
        f(6,6) + M(5), <- last line is "ggggggg" and minimum messiness of "aaa bbb c d ff"
    ) 

    note f(4,6) or "ee ff ggggggg" is not considered as it exceeds length along with all j <= 4 f(j,6)
*/

// concat v from [i,j]
string concat(vector<string>& v, int i, int j) {
    string res = "";
    for(int k = i; k <= j; k++) {
        res = res.empty() ? v[k] : (res + '_' + v[k]);
    }
    return res;
}

int word_break(string s, int length) {
    vector<string> words;
    stringstream ss(s);
    string temp;
    while(ss >> temp) words.push_back(temp);

    int N = words.size();
    vector<int> M(N), P(N);

    for(int i = 0; i < N; i++) {
        string last_line = "";
        int min_M = INT_MAX;
        int min_j = -1;
        for(int j = i; j >= 0; j--) {
            last_line = last_line.empty() ? words[j] : (words[j] + " " + last_line);

            // exceed maximum line length
            if(last_line.size() > length) break;

            int messiness_last_line = pow(length - last_line.size(), 2);
            int messiness_before_last_line = (j == 0) ? 0 : M[j-1];
            int partition_messiness = messiness_last_line + messiness_before_last_line;

            if(min_M > partition_messiness) {
                min_j = j;
                min_M = min(min_M, partition_messiness);
            }

        }

        P[i] = min_j;
        M[i] = min_M;
    }

    // reconstruct string
    stack<string> st;
    for(int i = N-1; i >= 0; i = P[i] - 1) {
        string word = concat(words, P[i], i);
        word += string(length - word.size(), '_');
        st.push(word);
    }

    while(!st.empty()) {
        cout << st.top() << endl;
        st.pop();
    }

    return M.back();
}

int main() {
    cout << word_break("aaa bbb c d ee ff ggggggg", 11) << endl;
    return 0;
}