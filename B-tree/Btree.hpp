#pragma once
#include "Node.hpp"
#include <algorithm>
#include <queue>
#include <vector>

class Btree {
public:
    Node* root;
    std::vector<Node*> loadedNodes; // Loaded nodes
    std::vector<int> freeNodes; // Deleted nodes offsets in pages.bin
    std::vector<int> freeRecords; // Deleted records offsets in records.bin

    Btree();

    void insertRecord();
    void updateRecord();
    void deleteRecord();
    void printRecord();
    void printTree();
    void printAllRecords();

    Node* findNode(int key);

    Element findElement(int key);
    void insertElement(Element element);

    void insertRecord(Record record);
    void updateRecord(Record record);
    void deleteRecord(int key); // TODO

    void fixup(Node* node);
    void fixOverflow(Node* node, Node* left, Node* right, int index);
    void fixUnderflow(Node* node, Node* left, Node* right, int index); // TODO

    void split(Node* node);
    void merge(); // TODO

    void printRecord(int key);
    void print(Node* node, const std::string& prefix = " ", bool isLast = true);
    void printReverse(Node* node, const std::string& prefix = " ", bool isLast = true);
    void printALL(Node* node);

    void clearFiles();
    void unloadNodes();
    void deleteTree();
};
