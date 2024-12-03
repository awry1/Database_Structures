#include "Btree.hpp"

Btree::Btree() {
    nodes = std::vector<Node>();
    freeNodes = std::vector<int>();
    freeRecords = std::vector<int>();

    Node root;
    nodes.push_back(root);
}

void Btree::findRecord() {
    std::cout << "Find record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
}

void Btree::insertRecord() {
    std::cout << "Insert record" << std::endl;
    Record record;
    record.readFromConsole();
}

void Btree::updateRecord() {
    std::cout << "Update record" << std::endl;
    Record record;
    record.readFromConsole();
}

void Btree::deleteRecord() {
    std::cout << "Delete record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
}

void Btree::printTree() {
    std::cout << "Print tree" << std::endl;
}
