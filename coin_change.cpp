#include <iostream>
 
using namespace std;

#define ROWS (5)
#define COLS (1001)
int dp[ROWS][COLS];


// dp[i][j] represents number of ways to get target sum j using coins [0,i]
// dp[0][j] = 0 because there are no coins left and there is still a target sum
// dp[i][0] = 1 because target sum is zero (we reached target sum) and there was still coins left
// dp[i][j] = dp[i-1][j] + dp[i][j-i]
//          = (we exclude this coin) + (we use the coin)
// equivalent to partitions
int fill_dp() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(i == 0) {
                dp[i][j] = 0;
            } else if(j == 0) {
                dp[i][j] = 1;
            } else {
                dp[i][j] = dp[i-1][j];
                if(j - i >= 0) {
                    dp[i][j] += dp[i][j-i];
                }
            }
        }
    }
}

int main() {
    return 0;
}