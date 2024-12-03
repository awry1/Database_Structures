#pragma once
#include <vector>
#include "Node.hpp"

class Btree {
public:
    std::vector<Node> nodes; // Loaded nodes, must include the root node
    std::vector<int> freeNodes; // Deleted nodes offsets in pages.bin
    std::vector<int> freeRecords; // Deleted records offsets in records.bin

    Btree();

    void findRecord();
    void insertRecord();
    void updateRecord();
    void deleteRecord();
    void printTree();
};
