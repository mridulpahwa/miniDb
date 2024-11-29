#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"


class BTree
{
    public:
    BTreeNode *root; // root of the Btree
    int t;  

    BTree(int t);
    BTreeNode* search(int key);

    void insert (int key, int value);
    void display();
    void deleteKey(int key);

    private:
    void insertNonFull(BTreeNode *node, int key, int value);
    void splitChild(BTreeNode *parent, int i, BTreeNode *fullChild);
    BTreeNode* searchRecursive(BTreeNode* node, int key);
    //helper methods for deletion
    void deleteRecursive(BTreeNode* node, int key);
    void removeFromLeaf(BTreeNode* node, int index);
    void deleteInternalNode(BTreeNode* node, int index);
    int getPredecessor(BTreeNode* node, int index);
    int getSuccessor(BTreeNode* node, int index);
    void mergeNodes(BTreeNode* parent, int index, BTreeNode* leftChild, BTreeNode* rightChild);
    BTreeNode* fixChild(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* leftSibling, BTreeNode* rightSibling);
    void borrowFromLeftSibling(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* leftSibling);
    void borrowFromRightSibling(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* rightSibling);


};

#endif // BTREE_H
