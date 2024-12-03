#include "Btree.hpp"

Btree::Btree() {
    nodes = std::vector<Node>();
    freeNodes = std::vector<int>();
    freeElements = std::vector<int>();

    Node root;
    nodes.push_back(root);
}
