#ifndef BTREENODE_H
#define BTREENODE_H

#include <iostream>

class BTreeNode
{
    public:
    bool leaf;   // is it a leaf node
    int *keys;   // array to hold keys, eg: column values in a database
    int *values; // array to hold values
    BTreeNode **children; // pointer to children node 
    int numKeys; // number of keys in the current node
    int maxKeys; // maximum no of keys a node can hold
    BTreeNode *parent; // parent of the current node

    BTreeNode(int t, bool leaf);

    BTreeNode(int maxKeys);

    ~BTreeNode();

};

#endif //BTREENODE_H