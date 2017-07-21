#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
 
using namespace std;
 
struct Node {
    int val, height;
    Node* parent, *left, *right;
    Node(int _val): val(_val), height(0), parent(NULL), left(NULL), right(NULL) {}
};
 
typedef Node TreeNode;
 
class TreePrinter {
    bool HasNonNullNodes(const vector<TreeNode*>& nodes) {
        return any_of(nodes.begin(), nodes.end(), [](const TreeNode* node) -> bool { return node != NULL; });
    }
 
    string GenerateWhiteSpaces(int count) {
        return string(max(count, 0), ' ');
    }
 
    void Print(vector<TreeNode*> nodes, int level, int max_level) {
        if(nodes.empty() || !HasNonNullNodes(nodes))
            return;
 
        int floor_level = max_level - level;
        int edge_lines = pow(2, max(floor_level - 1, 0));
        int first_spaces = pow(2, floor_level) - 1;
        int between_spaces = pow(2, floor_level + 1) - 1;
 
        string output = "";
        output += GenerateWhiteSpaces(first_spaces);
 
        vector<TreeNode*> new_nodes;
        for(const TreeNode* node: nodes) {
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
 
    int MaxLevel(TreeNode* node) {
        if (node == NULL) return 0;
        return 1 + max(MaxLevel(node -> left), MaxLevel(node -> right));
    }
public:
    void Print(TreeNode* root) {
        int max_level = MaxLevel(root);
        Print({root}, 1, max_level);
    }
};


class Tree {
    bool should_balance_;
    TreeNode* root_ = NULL;
    TreePrinter tree_printer_;
    // Rebalances tree by traversing from the inserted node all the way up to the root
    // and rotating based on height
    void Rebalance(TreeNode* node) {
        TreeNode* curr = node;
 
        while(curr) {
            UpdateHeight(curr);
 
            // Left Heavy
            if(GetHeight(curr -> left) >= GetHeight(curr -> right) + 2) {
                // left left child -> rotate right
                if(GetHeight(curr -> left -> left) >= GetHeight(curr -> left -> right)) {
                    RotateRight(curr);
                // left right child -> rotate left then rotate right
                } else {
                    RotateLeft(curr -> left);
                    RotateRight(curr);
                }
            // right heavy
            } else if(GetHeight(curr -> right) >= GetHeight(curr -> left) + 2) {
                // right right child -> rotate left
                if(GetHeight(curr -> right -> right) >= GetHeight(curr -> right -> left)) {
                    RotateLeft(curr);
                // right left child -> rotate right then rotate left
                } else {
                    RotateRight(curr -> right);
                    RotateLeft(curr);
                }
            }
 
            curr = curr -> parent;
        }
    }
 
    void RotateLeft(TreeNode* node) {
        TreeNode* y = node -> right;
        y -> parent = node -> parent;
 
        if(y -> parent == NULL) {
            root_ = y;
        } else {
            if(y -> parent -> left == node) {
                y -> parent -> left = y;
            } else if(y -> parent -> right == node) {
                y -> parent -> right = y;
            }
        }
 
        node -> right = y -> left;
        if(node -> right != NULL) {
            node -> right -> parent = node;
        }
 
        y -> left = node;
        node -> parent = y;
        UpdateHeight(node);
        UpdateHeight(y);
    }
 
    void RotateRight(TreeNode* node) {
        TreeNode* y = node -> left;
        y -> parent = node -> parent;
 
        if(y -> parent == NULL) {
            root_ = y;
        } else {
            if(y -> parent -> left == node) {
                y -> parent -> left = y;
            } else if(y -> parent -> right == node) {
                y -> parent -> right = y;
            }
        }
 
        node -> left = y -> right;
        if(node -> left != NULL) {
            node -> left -> parent = node;
        }
 
        y -> right = node;
        node -> parent = y;
        UpdateHeight(node);
        UpdateHeight(y);
    }
 
    // Update the heights by taking the max height of the left and right branches
    int UpdateHeight(TreeNode* node) {
        int left_height = GetHeight(node -> left),
            right_height = GetHeight(node -> right);
 
        node -> height = 1 + max(left_height, right_height);
    }
public:
    Tree(vector<int> values = {}, bool should_balance = true) {
        should_balance_ = should_balance;
        InsertValues(values);
    }
 
    // Exposes root as public for modifications
    TreeNode* GetRoot() {
        return root_;
    }
 
    // Prints the tree using the TreePrinter class
    void Print() {
        tree_printer_.Print(root_);
    }
 
    // Helper function to get the height when there is no node present
    int GetHeight(TreeNode* node) {
        return (node == NULL) ? -1 : node -> height;
    }
 
    // Inserts any number of elements into the tree
    void InsertValues(const vector<int>& values) {
        for(const int& value: values) {
            InsertValue(value);
        }
    }
 
    // Method for inserting a single new value into the tree
    void InsertValue(const int& value) {
        TreeNode* to_insert_node = new TreeNode(value);
        if(root_ == NULL) {
            root_ = to_insert_node;
        } else {
            TreeNode* parent_of_insert = NULL;
            TreeNode* node = root_;
            while(node) {
                parent_of_insert = node;
                if(node -> val >= value) {
                    node = node -> left;
                } else {
                    node = node -> right;
                }
            }
 
            if(parent_of_insert -> val >= value) {
                parent_of_insert -> left = to_insert_node;
            } else {
                parent_of_insert -> right = to_insert_node;
            }
 
            to_insert_node -> parent = parent_of_insert;
        }
 
        if(should_balance_)
            Rebalance(to_insert_node);
    }
};
 
void RandomTree(int num_nodes_min=4, int num_nodes_max=10, int node_val_min=1, int node_val_max=25, bool balanced=true) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);
   
    int N = uniform_int_distribution<int>(num_nodes_min, num_nodes_max)(engine);
    vector<int> node_values(N);
 
    uniform_int_distribution<int> random_numbers(node_val_min, node_val_max);
    generate(node_values.begin(), node_values.end(), [&random_numbers, &engine](){ return random_numbers(engine); });
 
    Tree tree(node_values, balanced);
    tree.Print();
}
 
int main() {
    RandomTree(10,15);
    return 0;
}