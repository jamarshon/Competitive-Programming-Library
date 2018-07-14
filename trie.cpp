#include <iostream>

using namespace std;

struct TrieNode {
    bool is_word;
    TrieNode* child[26];
    TrieNode() {
        is_word = false;
        for(int i = 0; i < 26; i++) child[i] = NULL;
    }
};

class Trie {
    TrieNode* root_;
public:
    /** Initialize your data structure here. */
    Trie() {
        root_ = new TrieNode();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        TrieNode* curr = root_;
        for(auto c: word) {
            if(curr -> child[c - 'a'] == NULL) curr -> child[c - 'a'] = new TrieNode();
            curr = curr -> child[c - 'a'];
        }
        curr -> is_word = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        TrieNode* curr = root_;

        for(int i = 0, N = word.size(); i < N; i++) {
            if(curr -> child[word[i] - 'a'] == NULL) return false;
            curr = curr -> child[word[i] - 'a'];
        }

        return curr -> is_word;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        TrieNode* curr = root_;
        
        for(int i = 0, N = prefix.size(); i < N; i++) {
            if(curr -> child[prefix[i] - 'a'] == NULL) return false;
            curr = curr -> child[prefix[i] - 'a'];
        }

        return true;
    }
};

int main() {
    return 0;
}