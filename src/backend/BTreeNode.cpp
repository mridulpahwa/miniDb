#include "BTreeNode.h"

BTreeNode::BTreeNode(int t, bool leaf)
{
    maxKeys = 2*t -1;
    keys = new int[maxKeys];
    values = new int[maxKeys];
    children = new BTreeNode*[maxKeys + 1];
    numKeys = 0;
    this -> leaf = leaf;
    parent = nullptr;
}

BTreeNode::BTreeNode(int maxKeys)
{
    this -> maxKeys = maxKeys;
    this -> keys = new int[maxKeys];
    this -> values = new int[maxKeys];
    this -> children = new BTreeNode*[maxKeys + 1]; // one more child than keys (fundamental propery of Btree)
    this -> numKeys = 0;
    this -> leaf = true;
    this -> parent = nullptr;
}

BTreeNode::~BTreeNode()
{
    delete[] keys;
    delete [] values;
    delete[] children;
}