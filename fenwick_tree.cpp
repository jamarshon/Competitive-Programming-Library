#include <iostream>
#include <vector>

using namespace std;

class FenwickTree {
    vector<int> tree;
    int N;
    
    int lsb(int x){ return x & ~(x-1); }
    
    // [1,i]
    int query(int i) {
        int sum = 0;
        while(i != 0) {
            sum += tree[i];
            i -= lsb(i);
        }
        return sum;
    }
public:
    FenwickTree(vector<int> nums) {
        N = nums.size();
        tree.resize(N+1);
        
        tree[0] = 0;
        for(int i = 1; i <= N; i++) tree[i] = nums[i-1];
        
        for(int i = 1; i <= N; i++) {
            int parent = i + lsb(i);
            if(parent <= N) tree[parent] += tree[i];
        }
    }
    
    void update(int i, int val) {
        int delta = val - sumRange(i,i);
        i++;
        while(i <= N) {
            tree[i] += delta;
            i += lsb(i);
        }
    }
    
    int sumRange(int i, int j) {
        return query(j+1) - query(i);
    }
};

int main() {
    return 0;
}