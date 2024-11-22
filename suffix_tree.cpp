#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class SuffixTreeNode {
public:
    unordered_map<char, SuffixTreeNode*> children;
    int start, *end;
    int suffixIndex;

    SuffixTreeNode(int start, int* end) {
        this->start = start;
        this->end = end;
        this->suffixIndex = -1;
    }
};

class SuffixTree {
public:
    string text;
    SuffixTreeNode* root;
    int size;

    SuffixTree(string s) {
        text = s;
        size = s.length();
        root = new SuffixTreeNode(-1, new int(-1)); // root node
    }

    void buildTree() {
        for (int i = 0; i < size; i++) {
            buildTreeUtil(i, size - 1, i);
        }
    }

    void buildTreeUtil(int start, int end, int suffixIndex) {
        SuffixTreeNode* currentNode = root;

        for (int i = start; i <= end; i++) {
            char currentChar = text[i];

            if (currentNode->children.find(currentChar) == currentNode->children.end()) {
                SuffixTreeNode* newNode = new SuffixTreeNode(i, new int(end));
                currentNode->children[currentChar] = newNode;
            }
            currentNode = currentNode->children[currentChar];
        }

        currentNode->suffixIndex = suffixIndex;
    }

    void display(SuffixTreeNode* node, int level) {
        if (!node) return;

        for (int i = 0; i < level; i++) {
            cout << "  ";
        }

        if (node->suffixIndex != -1) {
            cout << "Suffix: " << text.substr(node->start, *(node->end) - node->start + 1)
                 << ", Suffix Index: " << node->suffixIndex << endl;
        }

        for (auto& child : node->children) {
            display(child.second, level + 1);
        }
    }
};

int main() {
    string text;
    cout << "Enter the text: ";
    getline(cin, text);

    // Create a suffix tree
    SuffixTree suffixTree(text);
    suffixTree.buildTree();  // Build the tree

    // Display the tree
    cout << "\nSuffix Tree for the given text:\n";
    suffixTree.display(suffixTree.root, 0);

    return 0;
}
