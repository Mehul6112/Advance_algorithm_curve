#include <iostream>
using namespace std;

// A BTree Node
class BTreeNode {
public:
    int* keys;      // Array of keys
    int t;          // Minimum degree (defines the range for the number of keys)
    BTreeNode** children; // Array of child pointers
    int n;          // Current number of keys
    bool isLeaf;    // True if the node is a leaf

    // Constructor
    BTreeNode(int t, bool isLeaf);

    // Traverse the nodes in the BTree in-order
    void traverse();

    // Insert a new key into a non-full node
    void insertNonFull(int k);

    // Split the child y of this node
    void splitChild(int i, BTreeNode* y);

    // Give BTree access to private members
    friend class BTree;
};

// A BTree
class BTree {
public:
    BTreeNode* root; // Pointer to the root node
    int t;           // Minimum degree

    // Constructor
    BTree(int t) {
        root = nullptr;
        this->t = t;
    }

    // Traverse the tree
    void traverse() {
        if (root != nullptr)
            root->traverse();
    }

    // Insert a new key into the BTree
    void insert(int k);
};

// BTreeNode Constructor
BTreeNode::BTreeNode(int t, bool isLeaf) {
    this->t = t;
    this->isLeaf = isLeaf;
    keys = new int[2 * t - 1];
    children = new BTreeNode*[2 * t];
    n = 0; // Initially no keys
}

// Traverse the tree rooted at this node
void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        // If this is not a leaf, traverse the child before printing the key
        if (!isLeaf)
            children[i]->traverse();
        cout << keys[i] << " ";
    }

    // Print the subtree rooted at the last child
    if (!isLeaf)
        children[i]->traverse();
}

// Insert a new key into a non-full node
void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (isLeaf) {
        // Find the location for the new key and shift keys as needed
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the new key
        keys[i + 1] = k;
        n++;
    } else {
        // Find the child that will receive the new key
        while (i >= 0 && keys[i] > k)
            i--;

        // Check if the found child is full
        if (children[i + 1]->n == 2 * t - 1) {
            // Split the child
            splitChild(i + 1, children[i + 1]);

            // Decide which of the two children will receive the new key
            if (keys[i + 1] < k)
                i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

// Split the child y of this node
void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->t, y->isLeaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    // Copy the last t children of y to z
    if (!y->isLeaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->n = t - 1; // Reduce the number of keys in y

    // Create space for the new child in this node
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Link the new child to this node
    children[i + 1] = z;

    // Move keys in this node to make space for the middle key
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[t - 1];

    n++; // Increment the count of keys in this node
}

// Insert a new key into the BTree
void BTree::insert(int k) {
    if (root == nullptr) {
        // Allocate memory for the root
        root = new BTreeNode(t, true);
        root->keys[0] = k; // Insert the key
        root->n = 1;       // Initially one key
    } else {
        // If the root is full, grow the tree
        if (root->n == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);

            // Make the old root a child of the new root
            s->children[0] = root;

            // Split the old root and move a key to the new root
            s->splitChild(0, root);

            // Decide which of the two children gets the new key
            int i = (s->keys[0] < k) ? 1 : 0;
            s->children[i]->insertNonFull(k);

            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
}

// Main function to demonstrate BTree functionality
int main() {
    BTree t(3); // A BTree with minimum degree 3

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of the constructed BTree is: ";
    t.traverse();
    cout << endl;

    return 0;
}
