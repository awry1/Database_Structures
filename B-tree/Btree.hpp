#pragma once
#include <vector>
#include "Node.hpp"

class Btree {
public:
    std::vector<Node> nodes; // loaded nodes, must include the root node
    std::vector<int> freeNodes; // offsets of free nodes in the file
    std::vector<int> freeElements; // offsets of free elements in the file

    Btree();
};
