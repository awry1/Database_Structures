#pragma once
#include "Node.hpp"
#include <algorithm>
#include <queue>
#include <vector>
constexpr auto FREE_NODES_FILE = "free_nodes.txt";
constexpr auto FREE_RECORDS_FILE = "free_records.txt";

class Btree {
public:
    Node* root;
    std::vector<Node*> loadedNodes; // Loaded nodes
    std::vector<int> freeNodes; // Deleted nodes offsets in pages.bin
    std::vector<int> freeRecords; // Deleted records offsets in records.bin
    bool display;
    double reads;
    double writes;

    Btree(bool display);

    void insertRecord();
    void updateRecord();
    void deleteRecord();
    void printRecord();
    void printTree();
    void printAllRecords();

    Node* findNode(int key);

    Element findElement(int key);
    void insertElement(Element element);
    void deleteElement(Element element);

    void insertRecord(Record record);
    void updateRecord(Record record);
    void deleteRecord(int key);

    void fixup(Node* node);
    void fixOverflow(Node* node, Node* left, Node* right, int index);
    void fixUnderflow(Node* node, Node* left, Node* right, int index);

    void split(Node* node);
    void merge(Node* node, Node* left, Node* right, int index);

    void printRecord(int key);
    void print(Node* node, const std::string& prefix = " ", bool isLast = true);
    void printReverse(Node* node, const std::string& prefix = " ", bool isLast = true);
    void printALL(Node* node);

    void reorganizeFiles();
    void clearFiles();
    void unloadNodes();
    void deleteTree();
};
