#include <iostream>
#include "BTree.h"
#include "Row.h"
#include "BTreeNode.h"

void testInsertAndTable() {
    // Create a BTree with degree 3 (example)
    BTree bTree(3);

    // Create some rows
    Row row1, row2, row3;
    row1.setColumn("id", 1); // Primary Key
    row1.setColumn("name", std::string("Alice"));
    row1.setColumn("age", 20);

    row2.setColumn("id", 2); // Primary Key
    row2.setColumn("name", std::string("Bob"));
    row2.setColumn("age", 25);

    row3.setColumn("id", 3); // Primary Key
    row3.setColumn("name", std::string("Charlie"));
    row3.setColumn("age", 22);

    // Insert rows into the BTree
    std::cout << "Inserting rows into the BTree...\n";
    bTree.insertRow(row1);
    bTree.insertRow(row2);
    bTree.insertRow(row3);

    // Display the tree to verify insertion
    std::cout << "BTree after row insertions:\n";
    bTree.display();
}

void testRowRetrieval() {
    BTree bTree(3);

    // Insert some rows
    Row row1, row2;
    row1.setColumn("id", 1); // Primary Key
    row1.setColumn("name", std::string("Alice"));
    row1.setColumn("age", 20);

    row2.setColumn("id", 2); // Primary Key
    row2.setColumn("name", std::string("Bob"));
    row2.setColumn("age", 25);

    bTree.insertRow(row1);
    bTree.insertRow(row2);

}

int main() {
    std::cout << "Running tests for BTree and Row functionalities...\n";

    testInsertAndTable();

    std::cout << "All tests complete.\n";
    return 0;
}
