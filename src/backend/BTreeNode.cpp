#include "BTreeNode.h"

BTreeNode::BTreeNode(int t, bool leaf)
{
    maxKeys = 2*t -1;
    keys = new int[maxKeys];
    row = new Row*[maxKeys];
    children = new BTreeNode*[maxKeys + 1];
    numKeys = 0;
    this -> leaf = leaf;
    parent = nullptr;
}

BTreeNode::BTreeNode(int maxKeys)
{
    this -> maxKeys = maxKeys;
    this -> keys = new int[maxKeys];
    this -> row = new Row *[maxKeys];
    this -> children = new BTreeNode*[maxKeys + 1]; // one more child than keys (fundamental propery of Btree)
    this -> numKeys = 0;
    this -> leaf = true;
    this -> parent = nullptr;
}

BTreeNode::~BTreeNode()
{
    delete[] keys;
    delete [] row;
    delete[] children;
}