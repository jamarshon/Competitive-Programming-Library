#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <queue>
#include <algorithm>
#include <stack>
#include <functional>
#include <unordered_map>
#include <cassert>
#include <bitset>
#include <random>
#include <chrono>
 
using namespace std;

struct CTreeNode {
    int val;
    CTreeNode* left, *right;
    CTreeNode(int _val): val(_val), left(NULL), right(NULL) {}
};

// root is the minimum of whole array (index at i_min) and left subtree is a cartesian tree built from
// 0 to i_min and right subtree from i_min + 1 to N recursively
// the value inside the node is the index of the element
CTreeNode* BuildCartesianTree(const vector<int>& nums) {
    vector<CTreeNode*> stk; // use vector instead of stack to get first element

    int N = nums.size();

    for(int i = 0; i < N; i++) {
        CTreeNode* curr = new CTreeNode(i);
        CTreeNode* left = NULL;

        // find last element in stack that is <= A[i] and make it the left child of current
        while(!stk.empty() && nums[stk.back() -> val] > nums[i]) {
            left = stk.back();
            stk.pop_back();
        }

        curr -> left = left;

        // if stack is not empty (ie. this node is the root) then have the last node in stack's
        // right child to be this node 
        if(!stk.empty()) {
            stk.back() -> right = curr;
        }
        stk.push_back(curr);
    }

    return stk.front(); // the root as nothing is smaller than it
}

// E[1, 2*N-1] – the nodes visited in an Euler Tour of T; E[i] is the label of i-th visited node in the tour
// L[1, 2*N-1] – the levels of the nodes visited in the Euler Tour; L[i] is the level of node E[i]
// H[1, N] – H[i] is the index of the first occurrence of node i in E (any occurrence would be good, so it’s not bad if we consider the first one)
// E[RMQ(L, H[i], H[j])] is LCA or index of minimum of Cartesian tree
void EulerTour(CTreeNode* root, vector<int>& E, vector<int>& L, vector<int>& H, int level) {
    if(!root) return;
    H[root -> val] = E.size();
 
    L.push_back(level);
    E.push_back(root -> val);
 
    if(root -> left) {
        EulerTour(root -> left, E, L, H, level + 1);
 
        L.push_back(level);
        E.push_back(root -> val);
    }
 
    if(root -> right) {
        EulerTour(root -> right, E, L, H, level + 1);
 
        L.push_back(level);
        E.push_back(root -> val);
    }
}
 
vector<vector<int>> SparseTable(vector<int> A) {
    int N = A.size();
 
    int M_len = log2(N) + 1;
    vector<vector<int>> M(N, vector<int>(M_len));
    int i, j;
    for (i = 0; i < N; i++)
        M[i][0] = i;
 
    for (j = 1; 1 << j <= N; j++) {
        for (i = 0; i + (1 << (j - 1)) < N; i++) {
            if (A[M[i][j - 1]] < A[M[i + (1 << (j - 1))][j - 1]]) {
                M[i][j] = M[i][j - 1];
            } else {
                M[i][j] = M[i + (1 << (j - 1))][j - 1];
            }
        }
    }     
            
    return M;
}
 
// divide A into blocks
// For each block compute the minimum and it's index
// For each block, normalize by first element and convert -1 to 1,
//      use this as an integer as a key to find in T what kind of block it is
// For each block type, compute the minimum between each index i,j which are relative to the
//      the block so i,j < block size in table P[i][j]
// Given a query, i,j
//      i and j are in the same block, so we use the value computed in P and T
//      i and j are in different blocks,  so we compute three values:
//          the minimum from i to the end of i’s block using P and T,
//          the minimum of all blocks between i’s and j‘s block using precomputed queries 
//              on A’ and the minimum from the begining of j’s block to j, again using T and P;
//              finally return the position where the overall minimum is using the three values you just computed.
 
function<int(int, int)> RestrictedRMQ(const vector<int>& A) {
    int N = A.size();
    int block_size = max(ceil(log2(N)/2.0), 1.0);
    int condensed_arr_size = ceil((double)N/block_size);
    int A_diff[N];
 
    vector<int> min_block(condensed_arr_size),
        min_block_index(condensed_arr_size);
 
    for(int i = 0; i < N; i++) {
        A_diff[i] = (i > 0) ? A[i] - A[i-1]: 0;
        if(i % block_size == 0 || A[i] < A[min_block_index[i/block_size]]) {
            min_block_index[i/block_size] = i;
        }
        min_block[i/block_size] = A[min_block_index[i/block_size]];
    }
 
    vector<vector<int>> M = SparseTable(min_block);
 
    function<int(int, int)> st_query = [min_block, M](const int i, const int j) {
        int k = log2(j - i + 1);
        if(min_block[M[i][k]] <= min_block[M[j - (1 << k) + 1][k]]) {
            return M[i][k];
        } else {
            return M[j - (1 << k) + 1][k];
        }
    };
 
    vector<int> T(condensed_arr_size);
    for(int i = 0; i < N;) {
        int target = i + block_size;
        int index = i/block_size;
        T[index] = 0;
 
        while(++i < target) {
            int item;
            if(i >= N) {
                item = 1;
            } else {
                item = (A_diff[i] == -1) ? 0 : 1;
            }
            T[index] = (T[index] << 1) | item;
        }
    }
 
    int max_possible_block_type = pow(2, block_size - 1) - 1;
    vector<vector<vector<int>>> P(max_possible_block_type + 1, vector<vector<int>>(condensed_arr_size, vector<int>(condensed_arr_size)));
 
    vector<int> temp(block_size);
    for(int i = 0; i <= max_possible_block_type; i++) {
        int key = i;
        for(int j = 0; j < block_size; j++) {
            temp[block_size - j - 1] = key & 1;
            key >>= 1;
        }
 
        for(int j = 1; j < block_size; j++) {
            temp[j] = temp[j-1] + ((temp[j] == 0) ? -1 : 1);   
        }
 
        for(int j = 0; j < block_size; j++)
            P[i][j][j] = j;
 
        for(int j = 0; j < block_size; j++) {
            for(int k = j + 1; k < block_size; k++) {
                if(temp[P[i][j][k - 1]] < temp[k]) {
                    P[i][j][k] = P[i][j][k - 1];
                } else {
                    P[i][j][k] = k;
                }
            }
        }
    }
 
    function<int(int, int)> query = [block_size, st_query, A, min_block_index, T, P](int i, int j) {
        if(i > j) swap(i, j);
 
        int start_block = ceil((double)i/block_size);
        int end_block = j/block_size - (j % block_size != block_size - 1);
        int res = -1;
 
        if(i/block_size == j/block_size) {
            // i and j are in the same block, so we use the value computed in P and T
            res = (i/block_size)*block_size + P[T[i/block_size]][i % block_size][j % block_size];
        } else {
            // i and j are in different blocks,  so we compute three values: the minimum from i to the
            // end of i’s block using P and T, the minimum of all blocks between i’s and j‘s block using
            // precomputed queries  on A’ and the minimum from the begining of j’s block to j, again using T and P;
            // finally return the position where the overall minimum is using the three values you just computed.
            if(start_block <= end_block) {
                int min_block_ind = st_query(start_block, end_block);
                int arr_min_block_ind = min_block_index[min_block_ind];
                if(res == -1 || A[res] < A[arr_min_block_ind]) res = arr_min_block_ind;
            }
           
            if(start_block*block_size != i) {
                int pre_ind = (start_block - 1)*block_size + P[T[start_block - 1]][i % block_size][block_size - 1];
                if(res == -1 || A[pre_ind] < A[res]) res = pre_ind;
            }
 
            if((end_block + 1)*block_size - 1 != j) {
                int end_ind = (end_block + 1)*block_size + P[T[end_block + 1]][0][j % block_size];
                if(res == -1 || A[end_ind] < A[res]) res = end_ind;
            }
        }
 
        return res;
    };
 
    return query;
}

void test(const vector<int>& A) {
    int N = A.size();
    CTreeNode* root = BuildCartesianTree(A);
    P2<CTreeNode> p;
    p.Print(root);
    cout << endl;
    vector<int> E, L;
    vector<int> H(N);
 
    E.reserve(2*N);
    L.reserve(2*N);
 
    EulerTour(root, E, L, H, 0);
    assert(E.size() == 2*N-1);
    assert(L.size() == 2*N-1);
 
    function<int(int, int)> query = RestrictedRMQ(L);
 
    for(int i = 0; i < N; i++) {
        for(int j = i; j < N; j++) {
            int min_el = i;
            for(int k = i; k <= j; k++) {
                if(A[k] < A[min_el]) min_el = k;
            }
 
            int res = query(H[i], H[j]);
            assert(A[E[res]] == A[min_el]);
        }
    }
}
 
int main() {
    vector<int> A{2,4,3,1,6,7,8,9,1,7};
    test(A);
    return 0;
    for(int i = 0; i < 1000; i++) {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine engine(seed);
       
        int N = uniform_int_distribution<int>(1, 100)(engine);
 
        A.clear();
        A.resize(N);
        uniform_int_distribution<int> dist(-100, 100);
        auto gen = bind(dist, engine);
        generate(A.begin(), A.end(), gen);
        for(auto l: A) cout << l << ", "; cout << endl;
        test(A);
        cout << "PASS: #" << i << endl;
    }
   
    return 0;
}