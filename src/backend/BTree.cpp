#include "Btree.h"
#include "BTreeNode.h"
#include <iostream>
#include <queue>

BTree::BTree(int t)
{
   root = new BTreeNode(t, true);
   this -> t = t;

}

void BTree::display()
{
    if (!root || root->numKeys == 0)
    {
        std::cout << "Tree is empty!!" << std::endl;
        return;
    }

    std::queue<BTreeNode *> q;
    q.push(root);
    q.push(nullptr); // Level marker

    while (!q.empty())
    {
        BTreeNode *node = q.front();
        q.pop();

        if (node == nullptr)
        {
            // End of the current level
            std::cout << std::endl;
            if (!q.empty())
                q.push(nullptr); // Add level marker for next level
            continue;
        }

        for (int i = 0; i < node->numKeys; i++)
        {
            std::cout << node->keys[i] << " ";
        }
        std::cout << "| ";

        if (!node->leaf)
        {
            for (int i = 0; i <= node->numKeys; i++)
            {
                q.push(node->children[i]);
            }
        }
    }
    std::cout << std::endl;
}

void BTree::insert(int key, Row *row)
{
    //If the root node is full, we need to split it
    if (root -> numKeys == root -> maxKeys)
    {
        BTreeNode *newNode = new BTreeNode(t, false);  // Create a new non-leaf node
        newNode -> children[0] = root;
        root -> parent = newNode;
        splitChild(newNode, 0 , root);

        root = newNode;
    }

    insertNonFull(root, key, row);
}

void BTree::insertNonFull(BTreeNode *node, int key, Row *row)
{
    int i = node -> numKeys -1;

    //if it is a leaf node, just insert the key and value
    if (node -> leaf)
    {
        //Insert the key in the correct position in the node
        while (i >= 0 && key < node -> keys[i])
        {
            node -> keys[i+1] = node -> keys[i];
            node -> row[i+1] = node -> row[i];
            i--;
        }
        node -> keys[i+1] = key;
        node -> row[i+1] = row;
        node -> numKeys++;
    }
    else
    //Find the child that should have the key
    {
        while (i >= 0 && key < node -> keys[i] )
        {
            i--;
        }
        i++;

        //check if the child is full
        if (node -> children[i] -> numKeys == 2*t -1)
        {
            splitChild(node, i, node -> children[i]);
            if (key > node -> keys[i])
                i++;
        }
    
        insertNonFull(node -> children[i], key, row);
    }
}

void BTree::splitChild(BTreeNode *parent, int childIndex, BTreeNode* child)
{
    //Create a new node to hold the last (t-1) keys of the child
    BTreeNode *newChild = new BTreeNode(t, child->leaf);
    newChild -> numKeys = t-1;

    //Copy the last (t-1) keys from the child to the new child
    for(int i =0; i<t-1 ; i++)
    {
        newChild -> keys[i] = child-> keys[i+t];
        newChild -> row[i] = child -> row[i+t];
    }

    // copy the last children if the child is not a leaf
    if(!child -> leaf)
    {
        for (int i=0; i < t; i++)
            newChild -> children[i] = child -> children[i+t];
    }

    child -> numKeys = t-1;

    for (int i = parent->numKeys; i >= childIndex +1; i--)
    {
        parent -> children[i+1] = parent ->children[i];
    }

    // Insert the new child into the parent's children array
    parent->children[childIndex + 1] = newChild;

    for (int i = parent -> numKeys -1; i >= childIndex; i--)
    {
        parent -> keys[i+1] = parent->keys[i];
        parent -> row[i+1] = parent -> row[i];
    }

    parent->keys[childIndex] = child -> keys[t-1];
    parent -> row[childIndex] = child -> row[t-1];
    parent -> numKeys +=1;
}

BTreeNode* BTree::search(int key)
{
    if(!root)
    {
        std::cout<< "Tree is empty!" << std::endl;
        return nullptr;
    }
    return searchRecursive(root, key);
}

BTreeNode* BTree::searchRecursive(BTreeNode* node, int key)
{
    //Find the first key that is key>=key
    if (node == nullptr)
        return nullptr;

    int i = 0;
    while (i < node -> numKeys && key > node -> keys[i])
        i++;

    //if key is found, return the node
    if (i < node -> numKeys && key == node ->keys[i])
        return node;
    
    // if the key is not found and node is a leaf node
    if(node -> leaf)
        return nullptr;

    return searchRecursive(node -> children[i], key);
    
}

void BTree::insertRow(const Row& row)
{
    // Extracting the key(assuming first column is primary key) 
    int key = row.getPrimaryKey();

    // if the root node is full, split it
    if (root -> numKeys == root -> maxKeys)
    {
        BTreeNode* newNode = new BTreeNode(t, false);
        newNode -> children[0] = root;
        root -> parent = newNode;

        splitChild(newNode, 0, root);
        root = newNode; 
    }

    //Insert row into correct position in the tree
    insertRowNonFull(root, key, row);
}

void BTree::insertRowNonFull(BTreeNode *node, int key,const Row& row)
{
    int i = node -> numKeys - 1;

    // if node is a lead, insert it directly
    if (node -> leaf)
    {
        while ( i >= 0 && key < node -> keys[i])
        {
            node -> keys[i+1] = node -> keys[i];
            node -> row[i+1] = node -> row[i];
            i--;
        }

        node -> keys[i + 1] = key;
        node -> row[i + 1] = new Row(row);
        node -> numKeys++;
    }
    else
    {
        //Find the chid for the key
        while (i >= 0 && key < node -> keys[i])
            i--;
        
        i++;

        //if the child is full, split it!
        if (node -> children[i] -> numKeys == 2 * t - 1 )
        {
            splitChild(node, i, node -> children[i]);

            //After the split, decide which child to insert the row into
            if(key > node -> keys[i])
                i++;
        }

        insertRowNonFull( node -> children[i], key, row);
        
    }
}

void BTree::deleteKey(int key)
{
    if (!root)
    {
        std::cout<<"Tree is empty!"<< std::endl;
        return;
    }

    deleteRecursive(root, key);

    if(root -> numKeys == 0 && !root -> leaf)
    {
        BTreeNode * oldRoot = root;
        root = root -> children[0];

        delete oldRoot;  // delete oldRoot to prevent memory loss
    }
}

void BTree::deleteRecursive(BTreeNode* node, int key)
{
    int i =0;

    while (i < node -> numKeys && key > node -> keys[i])
        i++;
    
    std::cout << "Key: " << key << " in node: ";
    for (int j = 0; j < node->numKeys; j++)
        std::cout << node->keys[j] << " ";
    std::cout << "\n";

    if (i < node -> numKeys &&  key == node -> keys[i])
    {  
        std::cout << "Key found in current node.\n";
        // Case 1: node is a leaf
        if (node -> leaf)
        {
            std::cout << "Case 1: Removing key from leaf.\n";
            removeFromLeaf(node, i);

        }else
        {
            std::cout << "Case 2: Removing key from internal node.\n";

            // Case 2: node is an internal node
            deleteInternalNode(node, i);
        }
        return;
    } 
    else
    {
        std::cout << "Key not in current node. Descending into child " << i << ".\n";
        //Case 3: key is not in the current node, go down the tree
        if (node -> leaf)
        {
            std::cout<< "Key not found in the tree" << std::endl; 
            return;
        }
        BTreeNode* child = node->children[i];
        BTreeNode* leftSibling = (i > 0) ? node->children[i - 1] : nullptr;
        BTreeNode* rightSibling = (i < node->numKeys) ? node->children[i + 1] : nullptr;
        // if child has fewer than t-1 keys, rebalance by merging or borrowing
        if (child -> numKeys < t)
        {
            std::cout << "Child has less than t-1 keys. Fixing child.\n";
             child = fixChild(node, i, child, leftSibling, rightSibling);

        }   
        if (i < node->numKeys && node->keys[i] == key)
        {
            deleteRecursive(node, key); // Key could be in the parent node
        }
        else
        {
            deleteRecursive(child, key);
        }

    }

}

void BTree::removeFromLeaf(BTreeNode* node, int index)
{
    for (int i = index; i < node -> numKeys; i++)
    {
        node -> keys[i] = node -> keys[i+1];
    }
    
    node -> numKeys--; 

}   

void BTree::deleteInternalNode(BTreeNode* node, int index)
{
    int key = node -> keys[index];
    BTreeNode* leftChild = node -> children[index];
    BTreeNode* rightChild = node -> children[index+1];

    if (leftChild -> numKeys >= t)
        {
        // if left child has enough keys, move the predecessor key up
        int predKey = getPredecessor(node, index);
        std::cout << "Replacing key " << key << " with predecessor " << predKey << ".\n";
        node -> keys[index] = predKey;
        deleteRecursive(leftChild, predKey);
        }
        else if (rightChild -> numKeys >= t)
        {
        // if rightChild has enough keys, move the successor key up
        int succKey = getSuccessor(node, index);
        std::cout << "Replacing key " << key << " with successor " << succKey << ".\n";
        node -> keys[index] = succKey;
        deleteRecursive(rightChild, succKey);
        }
        else
        {
        std::cout << "Merging children to remove key " << key << ".\n";
        // if both children have less than t keys, merge them
        mergeNodes(node, index, leftChild, rightChild);
        deleteRecursive(leftChild, key);
        }

}

int BTree::getPredecessor(BTreeNode* node, int index)
{
    //Move to the left child of the given key
    BTreeNode* current = node -> children[index];

    //Traverse to the rightmost node of the subtree to find the successor
    while (!current -> leaf)
        current = current -> children[current -> numKeys];

    // The predecessor is the rightmost key of this node
    return current -> keys[current -> numKeys -1];
}

int BTree::getSuccessor(BTreeNode* node, int index)
{
    // The right child at node will provide us with the index
    BTreeNode* rightChild = node -> children[index + 1];
    
    // Traverse to the leftmost side of the right subtree to find the successor
    while (!rightChild -> leaf)
    {
        rightChild = rightChild -> children[0];
    }

    // The first key in the leftmost leaf is a successor
    return rightChild -> keys[0];
    
}

void BTree::mergeNodes(BTreeNode* parent, int index, BTreeNode* leftChild, BTreeNode* rightChild)
{
    std::cout << "Before merging at parent index " << index << ":\n";
    std::cout << "Parent keys: ";
    for (int i = 0; i < parent->numKeys; i++)
        std::cout << parent->keys[i] << " ";
    std::cout << "\n";

    std::cout << "Left child keys: ";
    for (int i = 0; i < leftChild->numKeys; i++)
        std::cout << leftChild->keys[i] << " ";
    std::cout << "\n";

    std::cout << "Right child keys: ";
    for (int i = 0; i < rightChild->numKeys; i++)
        std::cout << rightChild->keys[i] << " ";
    std::cout << "\n";
    // Move the key from the parent down to the left child (merge the parent key into the left child)
    leftChild -> keys[leftChild -> numKeys] = parent -> keys[index];
    leftChild -> numKeys++;

    // Move all keys from the right child to the left child
    for (int i =0; i < rightChild -> numKeys; i++)
    {
        leftChild -> keys[leftChild -> numKeys] = rightChild -> keys[i];
        leftChild -> numKeys++;
    }

    //Move all children of the right child to the left child (if the right child is not a leaf)
    if (!rightChild -> leaf)
    {
        for (int i =0; i <= rightChild -> numKeys; i++)
            leftChild -> children[leftChild -> numKeys + i] = rightChild -> children[i];
    }

    // Move the parent's pointer to the right child down, as it is now merged
    for (int i = index + 1; i < parent -> numKeys; i++)
        parent -> keys[i -1] = parent -> keys[i];

    for (int i = index + 2; i <= parent -> numKeys; i++)
        parent -> children[i-1] = parent -> children[i];
    
    // Decrement the number of keys in the parent
    parent -> numKeys--;    

    std::cout << "After merging:\n";
    std::cout << "Parent keys: ";
    for (int i = 0; i < parent->numKeys; i++)
        std::cout << parent->keys[i] << " ";
    std::cout << "\n";

    std::cout << "Merged left child keys: ";
    for (int i = 0; i < leftChild->numKeys; i++)
        std::cout << leftChild->keys[i] << " ";
    std::cout << "\n";

    delete rightChild; 

    std::cout << "Merged nodes at parent index " << index << ".\n";

}

BTreeNode* BTree::fixChild(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* leftSibling, BTreeNode* rightSibling)
{
    //Case 1: Borrow key from left sibling
    if (leftSibling && leftSibling -> numKeys >= t)
    {
        std::cout << "Borrowing from left sibling.\n";
        borrowFromLeftSibling(parent, index, child, leftSibling);
        return child;
    }
    // Case 2: Borrow key from right sibling
    else if(rightSibling && rightSibling -> numKeys >= t)
    {
        std::cout << "Borrowing from right sibling.\n";
        borrowFromRightSibling(parent, index, child, rightSibling);
        return child;
    }
    // Case 3: Merge with a sibling
    else 
    {
        if (leftSibling)
        {
            std::cout << "Merging with left sibling.\n";
            mergeNodes(parent, index -1 , leftSibling, child);
            return leftSibling; //Update the child pointer
        }
        else
        {
            std::cout << "Merging with right sibling.\n";
            mergeNodes(parent, index, child, rightSibling);
            return child;
        }
    }
    // Fallback to make sure valid pointer is returned
    std::cerr << "Error: fixChild could not resolve the issue.\n";
    return child;
}

void BTree::borrowFromLeftSibling(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* leftSibling)
{
    // Move the parent's key down to the child
    child -> keys[child -> numKeys] = parent -> keys[index -1];
    child -> numKeys +=1;

    // Move the key from the left sibling to the parent
    parent -> keys[index -1] = leftSibling -> keys[leftSibling -> numKeys -1];

    // Move the last child of the left sibling to the current child 
    if (!child -> leaf)
    {
        child -> children[child -> numKeys] = leftSibling -> children[leftSibling -> numKeys];
    }

    leftSibling -> numKeys -=1;
}

void BTree::borrowFromRightSibling(BTreeNode* parent, int index, BTreeNode* child, BTreeNode* leftSibling)
{
    // The right sibling is located at index + 1;
    BTreeNode* rightSibling = parent -> children[index + 1];

    //Move the right key from the parent to the child
    child -> keys[child -> numKeys] = parent -> keys[index];
    child -> numKeys++;

    //Move the first key from the right sibling to the parent
    parent -> keys[index] = rightSibling->keys[0];

    // Move the first child pointer from the right sibling to the child
    if (!rightSibling -> leaf)
        child -> children[child -> numKeys] = rightSibling -> children[0];

    // Remove the first key from the right sibling
    for(int i = 1; i < rightSibling -> numKeys; i++)
        rightSibling -> keys[i-1] = rightSibling -> keys[i];
    
    //Remove the first child pointer from the right sibling
    if (!rightSibling -> leaf)
    {
        for (int i = 1; i <= rightSibling -> numKeys; i++)
            rightSibling -> children[i-1] = rightSibling -> children[i];
    }

    rightSibling -> numKeys--;
}
