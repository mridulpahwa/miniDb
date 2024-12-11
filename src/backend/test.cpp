#include <iostream>
#include "BTree.h"
#include "Row.h"

int main() {
    BTree tree(3); // Minimum degree 3

    // Insert keys into the BTree
    std::cout << "Inserting keys into the BTree..." << std::endl;
    tree.insert(1, new Row());
    tree.insert(2, new Row());
    tree.insert(3, new Row());
    tree.insert(4, new Row());
    tree.insert(5, new Row());
    tree.insert(6, new Row());
    tree.insert(7, new Row());
    tree.insert(8, new Row());
    tree.insert(9, new Row());
    tree.insert(10, new Row());

    std::cout << "BTree structure after insertions:" << std::endl;
    tree.display();

    // Test search functionality
    std::cout << "Testing search functionality..." << std::endl;

    for (int key = 1; key <= 10; key++) {
        std::cout << "\nSearching for key " << key << "..." << std::endl;
        BTreeNode* result = tree.search(key);
        if (result) {
            std::cout << "Key " << key << " found in node with keys: ";
            for (int i = 0; i < result->numKeys; i++) {
                std::cout << result->keys[i] << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Key " << key << " not found!" << std::endl;
        }
    }

    // Test for a non-existent key
    int nonExistentKey = 15;
    std::cout << "\nSearching for a non-existent key (" << nonExistentKey << ")..." << std::endl;
    BTreeNode* result = tree.search(nonExistentKey);
    if (result) {
        std::cout << "Non-existent key " << nonExistentKey << " found in node with keys: ";
        for (int i = 0; i < result->numKeys; i++) {
            std::cout << result->keys[i] << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Non-existent key " << nonExistentKey << " not found!" << std::endl;
    }

    return 0;
}
