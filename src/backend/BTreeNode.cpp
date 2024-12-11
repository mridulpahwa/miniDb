#include "BTreeNode.h"

BTreeNode::BTreeNode(int t, bool leaf) {
    maxKeys = 2 * t - 1;
    keys = new int[maxKeys];
    row = new Row*[maxKeys];
    children = new BTreeNode*[maxKeys + 1];
    for (int i = 0; i < maxKeys + 1; i++) {
        children[i] = nullptr; // Initialize children to nullptr
    }
    numKeys = 0;
    this->leaf = leaf;
    parent = nullptr;
}

BTreeNode::~BTreeNode() {
    for (int i = 0; i < numKeys; i++) {
        delete row[i]; // Delete each Row object
    }
    for (int i = 0; i <= numKeys; i++) {
        delete children[i]; // Delete each child node
    }
    delete[] keys;
    delete[] row;
    delete[] children;
}
