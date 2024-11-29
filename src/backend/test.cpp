#include <iostream>
#include "BTree.h"

int main()
{
    // Test Case 1: Small Tree
    BTree btree1(2);
    btree1.insert(5, 50);
    btree1.insert(10, 100);
    btree1.insert(20, 200);

    std::cout << "BTree after insertions:\n";
    btree1.display();

    std::cout << "\nDeleting key 5:\n";
    btree1.deleteKey(5);
    btree1.display();

    std::cout << "\nDeleting key 10:\n";
    btree1.deleteKey(10);
    btree1.display();

    std::cout << "\nDeleting key 20:\n";
    btree1.deleteKey(20);
    btree1.display();

    // Test Case 2: Multi-Level Tree with Borrowing
    BTree btree2(2);
    btree2.insert(20, 200);
    btree2.insert(10, 100);
    btree2.insert(30, 300);
    btree2.insert(5, 50);
    btree2.insert(15, 150);
    btree2.insert(25, 250);
    btree2.insert(35, 350);

    std::cout << "\nBTree after insertions:\n";
    btree2.display();

    std::cout << "\nDeleting key 5:\n";
    btree2.deleteKey(5);
    btree2.display();

    std::cout << "\nDeleting key 25:\n";
    btree2.deleteKey(25);
    btree2.display();

    std::cout << "\nDeleting key 20:\n";
    btree2.deleteKey(20);
    btree2.display();

    // Test Case 3: Merging Nodes
    BTree btree3(2);
    btree3.insert(10, 100);
    btree3.insert(20, 200);
    btree3.insert(30, 300);
    btree3.insert(5, 50);
    btree3.insert(15, 150);
    btree3.insert(25, 250);
    btree3.insert(35, 350);
    btree3.insert(40, 400);

    std::cout << "\nBTree after insertions:\n";
    btree3.display();

    std::cout << "\nDeleting key 40:\n";
    btree3.deleteKey(40);
    btree3.display();

    std::cout << "\nDeleting key 35:\n";
    btree3.deleteKey(35);
    btree3.display();

    std::cout << "\nDeleting key 30:\n";
    btree3.deleteKey(30);
    btree3.display();

    std::cout << "\nDeleting key 25:\n";
    btree3.deleteKey(25);
    btree3.display();

    // Test Case 4: Nonexistent Key
    BTree btree4(2);
    btree4.insert(10, 100);
    btree4.insert(20, 200);
    btree4.insert(5, 50);

    std::cout << "\nBTree after insertions:\n";
    btree4.display();

    std::cout << "\nDeleting key 100 (nonexistent):\n";
    btree4.deleteKey(100);
    btree4.display();

    return 0;
}
