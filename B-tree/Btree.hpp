#pragma once
#include <queue>
#include <vector>
#include "Node.hpp"

class Btree {
public:
    Node root;
    std::vector<int> freeNodes; // Deleted nodes offsets in pages.bin
    std::vector<int> freeRecords; // Deleted records offsets in records.bin

    Btree();

    Node findNode(int key);
    Element findElement(int key);
    void findRecord();
    void findRecord(int key);
    void insertRecord();
    void insertRecord(Record record);
    void updateRecord();
    void updateRecord(Record record);
    void deleteRecord();
    void deleteRecord(int key);
    void printBFS();
    void printInOrder();
};

// TODO:
// check if nodes parents are remembered
//    if not, add loadedNodes vector to Btree
// when deleting a record, write it to freeRecords
//    also remove element containing the record from node
// when inserting a record, write it to records.bin
//    check freeRecords or append to the end
//    also add element containing the record to node
// when deleting a node, write it to freeNodes
// when inserting a node, write it to pages.bin
//    check freeNodes or append to the end
// after every operation, write modified nodes to pages.bin
// add printInOrder method
//