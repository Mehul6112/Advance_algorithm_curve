#include <iostream>
#include <sstream>
using namespace std;

// A BTree Node
class BTreeNode {
public:
    int* keys;            // Array of keys
    BTreeNode** children; // Array of child pointers
    int t;                // Minimum degree
    int n;                // Current number of keys
    bool isLeaf;          // True if this node is a leaf

    // Constructor to initialize a node
    BTreeNode(int t, bool isLeaf);

    // Search for a key in the subtree rooted at this node
    BTreeNode* search(int k);

    // Insert a key into a non-full node
    void insertNonFull(int k);

    // Split the child of this node
    void splitChild(int i, BTreeNode* y);

    // Display the keys in the BTree
    void traverse();

    // Allow access to private members from the BTree class
    friend class BTree;
};

// BTree class
class BTree {
public:
    BTreeNode* root; // Root of the BTree
    int t;           // Minimum degree

    // Constructor
    BTree(int t) {
        root = nullptr;
        this->t = t;
    }

    // Function to traverse the tree
    void traverse() {
        if (root != nullptr) root->traverse();
    }

    // Function to search for a key
    BTreeNode* search(int k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    // Function to insert a new key
    void insert(int k);

    // Function to insert multiple keys
    void insertMultiple(const string& keys);
};

// Constructor for BTreeNode
BTreeNode::BTreeNode(int t, bool isLeaf) {
    this->t = t;
    this->isLeaf = isLeaf;
    keys = new int[2 * t - 1]; // Maximum keys in a node
    children = new BTreeNode*[2 * t]; // Maximum children
    n = 0; // Start with 0 keys

    // Initialize keys to 0
    for (int i = 0; i < 2 * t - 1; i++) {
        keys[i] = 0;
    }

    // Initialize child pointers to nullptr
    for (int i = 0; i < 2 * t; i++) {
        children[i] = nullptr;
    }
}


// Search for a key in the subtree rooted at this node
BTreeNode* BTreeNode::search(int k) {
    int i = 0;

    // Find the first key greater than or equal to k
    while (i < n && k > keys[i]) i++;

    // If the key is found, return this node
    if (i < n && keys[i] == k) return this;

    // If this is a leaf, the key is not present
    if (isLeaf) return nullptr;

    // Recur to the appropriate child
    return children[i]->search(k);
}

// Traverse the BTree and display its keys
void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        // If this is not a leaf, traverse the child before the key
        if (!isLeaf && children[i]) {
            children[i]->traverse();
        }
        cout << keys[i] << " "; // Only print valid keys
    }

    // Print the subtree rooted at the last child
    if (!isLeaf && children[i]) {
        children[i]->traverse();
    }
}


// Insert a new key into the tree
void BTree::insert(int k) {
    if (!root) {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = k; // Insert key
        root->n = 1;       // Update the number of keys
    } else {
        // If root is full, grow the tree
        if (root->n == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);

            // Make old root a child of new root
            s->children[0] = root;

            // Split old root and update new root
            s->splitChild(0, root);

            // Insert key into the appropriate child
            if (s->keys[0] < k) s->children[1]->insertNonFull(k);
            else s->children[0]->insertNonFull(k);

            root = s; // Update root
        } else {
            // Root is not full, insert directly
            root->insertNonFull(k);
        }
    }
}

// Insert a key into a non-full node
void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (isLeaf) {
        // Find the position for the new key and move keys as needed
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    } else {
        // Find the child where the key should go
        while (i >= 0 && keys[i] > k) {
            i--;
        }

        // If the child is full, split it
        if (children[i + 1]->n == 2 * t - 1) {
            splitChild(i + 1, children[i + 1]);

            // After splitting, the middle key moves up and affects the child index
            if (keys[i + 1] < k) {
                i++;
            }
        }

        // Recur to the appropriate child
        children[i + 1]->insertNonFull(k);
    }
}


// Split a full child into two
void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->t, y->isLeaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
        y->keys[j + t] = 0; // Reset the moved keys in y
    }

    // Copy the children of y to z
    if (!y->isLeaf) {
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
            y->children[j + t] = nullptr; // Reset the moved children in y
        }
    }

    y->n = t - 1; // Reduce the number of keys in y

    // Shift children to make space for z
    for (int j = n; j >= i + 1; j--) {
        children[j + 1] = children[j];
    }
    children[i + 1] = z;

    // Shift keys to make space for the middle key
    for (int j = n - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
    }
    keys[i] = y->keys[t - 1];
    y->keys[t - 1] = 0; // Reset the middle key in y

    n++;
}


// Insert multiple keys separated by spaces
void BTree::insertMultiple(const string& keys) {
    stringstream ss(keys);
    int key;
    while (ss >> key) {
        insert(key);
    }
}

// Main function to demonstrate the program
int main() {
    BTree t(3); // BTree with minimum degree 3

    int choice;
    string input;
    while (true) {
        cout << "\n1. Insert keys\n2. Search\n3. Traverse\n4. Exit\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the newline character

        switch (choice) {
            case 1:
                cout << "Enter keys to insert (space-separated): ";
                getline(cin, input);
                t.insertMultiple(input);
                break;
            case 2:
                int key;
                cout << "Enter key to search: ";
                cin >> key;
                if (t.search(key)) cout << "Key found\n";
                else cout << "Key not found\n";
                break;
            case 3:
                cout << "Tree traversal: ";
                t.traverse();
                cout << endl;
                break;
            case 4:
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}
