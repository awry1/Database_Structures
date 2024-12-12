#pragma once
#include "Node.hpp"
#include <queue>
#include <vector>

class Btree {
public:
    Node* root;
    std::vector<Node*> loadedNodes; // Loaded nodes
    std::vector<int> freeNodes; // Deleted nodes offsets in pages.bin
    std::vector<int> freeRecords; // Deleted records offsets in records.bin
    int records; // Number of records in records.bin
    int nodes; // Number of nodes in pages.bin

    Btree();

    void findRecord();
    void insertRecord();
    void updateRecord();
    void deleteRecord();
    void printInOrder();
    void printTree();
    void printTreeReverse();
    void print(Node* node, const std::string& prefix = " ", bool isLast = true);
    void printReverse(Node* node, const std::string& prefix = " ", bool isLast = true);

    Node* findNode(int key);

    Element findElement(int key);
    void insertElement(Element element);

    void findRecord(int key);
    void insertRecord(Record record);
    void updateRecord(Record record);
    void deleteRecord(int key);

    void fixup(Node* node);

    void unloadNodes();
    void deleteTree();
};

// TODO:
// when deleting a record, write it to freeRecords
//    also remove element containing the record from node
// when deleting a node, write it to freeNodes
// when inserting a node, write it to pages.bin
//    check freeNodes or append to the end
// after every operation, write modified nodes to pages.bin
// add printInOrder method
// add compensation method
// add split method