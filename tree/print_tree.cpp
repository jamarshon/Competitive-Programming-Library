#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// p.printTree(root)
// p.PrintPreOrder(root)
template<typename T>
class P1 {public: int GetDepth(T* root) {if(root == NULL) return -1; return 1 + max(GetDepth(root -> left), GetDepth(root -> right)); } void Populate(T* root, vector<vector<string>>& res, int row, int start, int end, int& max_val) {if(root == NULL) return; if(start >= end) return; if(row >= res.size()) return; string val = to_string(root -> val); int mid = start + (end - start)/2; res[row][mid] = val; max_val = max(max_val, root -> val); Populate(root -> left, res, row + 1, start, mid, max_val); Populate(root -> right, res, row + 1, mid + 1, end, max_val); } string Pad(string s, int target_num_digits) {return string(max(target_num_digits - (int)s.size(), 0), ' ') + s; } void PrintTree(T* root) {int rd = GetDepth(root); int col = (1 << (rd + 1)) - 1; int max_val = 0; vector<vector<string>> res(rd + 1, vector<string>(col, "")); Populate(root, res, 0, 0, col, max_val); int target_num_digits = floor(log10(max_val)) + 1; for(auto row: res) {for(auto str: row) {cout << Pad(str, target_num_digits) << " "; } cout << endl; } } void PrintPreOrder(T* root) {queue<T*> q; q.push(root); while(!q.empty()) {int level = q.size(); for(int i = 0; i < level; i++) {T* front = q.front(); q.pop(); if(front) {cout << (front -> val) << ' '; q.push(front -> left); q.push(front -> right); } else {cout << "NULL "; } } cout << endl; } } };

// p.Print(root)
template<typename T>
class P2 {bool HasNonNullNodes(const vector<T*>& nodes) {return any_of(nodes.begin(), nodes.end(), [](const T* node) -> bool { return node != NULL; }); } string GenerateWhiteSpaces(int count) {return string(max(count, 0), ' '); } void Print(vector<T*> nodes, int level, int max_level) {if(nodes.empty() || !HasNonNullNodes(nodes)) return; int floor_level = max_level - level; int edge_lines = pow(2, max(floor_level - 1, 0)); int first_spaces = pow(2, floor_level) - 1; int between_spaces = pow(2, floor_level + 1) - 1; string output = ""; output += GenerateWhiteSpaces(first_spaces); vector<T*> new_nodes; for(const T* node: nodes) {if(node == NULL) {new_nodes.push_back(NULL); new_nodes.push_back(NULL); output += GenerateWhiteSpaces(1); } else {new_nodes.push_back(node -> left); new_nodes.push_back(node -> right); output += to_string(node -> val); } output += GenerateWhiteSpaces(between_spaces); } cout << output << endl; output = ""; for(int i = 1; i <= edge_lines; i++) {for(int j = 0; j < nodes.size(); j++) {output += GenerateWhiteSpaces(first_spaces - i); if (nodes[j] == NULL) {output += GenerateWhiteSpaces(2*edge_lines + i + 1); continue; } if (nodes[j] -> left == NULL) output += GenerateWhiteSpaces(1); else output += "/"; output += GenerateWhiteSpaces(i + i - 1); if (nodes[j] -> right == NULL) output += GenerateWhiteSpaces(1); else output += "\\"; output += GenerateWhiteSpaces(2*edge_lines - i); } cout << output << endl; output = ""; } Print(new_nodes, level + 1, max_level); } int MaxLevel(T* node) {if (node == NULL) return 0; return 1 + max(MaxLevel(node -> left), MaxLevel(node -> right)); } public: void Print(T* root) {int max_level = MaxLevel(root); Print({root}, 1, max_level); } };

template<typename T>
class Printer {
public:
    int GetDepth(T* root) {
        if(root == NULL) return -1;
        return 1 + max(GetDepth(root -> left), GetDepth(root -> right));
    }

    void Populate(T* root, vector<vector<string>>& res, int row, int start, int end, int& max_val) {
        if(root == NULL) return;
        if(start >= end) return;
        if(row >= res.size()) return;
        
        string val = to_string(root -> val);
        int mid = start + (end - start)/2;
        res[row][mid] = val;
        max_val = max(max_val, root -> val);
        
        Populate(root -> left, res, row + 1, start, mid, max_val);
        Populate(root -> right, res, row + 1, mid + 1, end, max_val);
    }

    string Pad(string s, int target_num_digits) {
        return string(max(target_num_digits - (int)s.size(), 0), ' ') + s;
    }

    void PrintTree(T* root) {
        // get the maximum depth of the tree
        int rd = GetDepth(root);
        int col = (1 << (rd + 1)) - 1; 
        int max_val = 0;

        // the matrix has depth rows and 2^(depth + 1) - 1 columns
        vector<vector<string>> res(rd + 1, vector<string>(col, "")); 
        Populate(root, res, 0, 0, col, max_val);

        int target_num_digits = floor(log10(max_val)) + 1;
        for(auto row: res) {
            for(auto str: row) {
                cout << Pad(str, target_num_digits) << " ";
            }
            cout << endl;
        }
    }

    void PrintPreOrder(T* root) {
        queue<T*> q;
        q.push(root);

        while(!q.empty()) {
            int level = q.size();
            for(int i = 0; i < level; i++) {
                T* front = q.front();
                q.pop();
                if(front) {
                    cout << (front -> val) << ' ';
                    q.push(front -> left);
                    q.push(front -> right);
                } else {
                    cout << "NULL ";
                }
            }
            cout << endl;
        }
    }
};


template<typename T>
class Printer2 {
    bool HasNonNullNodes(const vector<T*>& nodes) {
        return any_of(nodes.begin(), nodes.end(), [](const T* node) -> bool { return node != NULL; });
    }
 
    string GenerateWhiteSpaces(int count) {
        return string(max(count, 0), ' ');
    }
 
    void Print(vector<T*> nodes, int level, int max_level) {
        if(nodes.empty() || !HasNonNullNodes(nodes))
            return;
 
        int floor_level = max_level - level;
        int edge_lines = pow(2, max(floor_level - 1, 0));
        int first_spaces = pow(2, floor_level) - 1;
        int between_spaces = pow(2, floor_level + 1) - 1;
 
        string output = "";
        output += GenerateWhiteSpaces(first_spaces);
 
        vector<T*> new_nodes;
        for(const T* node: nodes) {
            if(node == NULL) {
                new_nodes.push_back(NULL);
                new_nodes.push_back(NULL);
                output += GenerateWhiteSpaces(1);
            } else {
                new_nodes.push_back(node -> left);
                new_nodes.push_back(node -> right);
                output += to_string(node -> val);
            }
 
            output += GenerateWhiteSpaces(between_spaces);
        }
 
        cout << output << endl;
        output = "";

        for(int i = 1; i <= edge_lines; i++) {
            for(int j = 0; j < nodes.size(); j++) {
 
                output += GenerateWhiteSpaces(first_spaces - i);
                if (nodes[j] == NULL) {
                    output += GenerateWhiteSpaces(2*edge_lines + i + 1);
                    continue;
                }
 
                if (nodes[j] -> left == NULL)
                    output += GenerateWhiteSpaces(1);
                else
                    output += "/";
 
                output += GenerateWhiteSpaces(i + i - 1);
 
                if (nodes[j] -> right == NULL)
                    output += GenerateWhiteSpaces(1);
                else
                    output += "\\";
 
                output += GenerateWhiteSpaces(2*edge_lines - i);
            }
 
            cout << output << endl;
            output = "";
        }
 
        Print(new_nodes, level + 1, max_level);
    }
 
    int MaxLevel(T* node) {
        if (node == NULL) return 0;
        return 1 + max(MaxLevel(node -> left), MaxLevel(node -> right));
    }
public:
    void Print(T* root) {
        int max_level = MaxLevel(root);
        Print({root}, 1, max_level);
    }
};

int main() {
    return 0;
}