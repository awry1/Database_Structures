#pragma once
#include <fstream>
#include "Element.hpp"
constexpr auto PAGES_FILE = "pages.bin";
constexpr auto NODE_DEGREE = 2;
constexpr auto MIN_ELEMENTS = NODE_DEGREE;
constexpr auto MAX_ELEMENTS = 2 * NODE_DEGREE;

class Element;

class Node {
public:
    int offset; // Node offset in pages.bin
    std::vector<Element> elements;
    std::vector<int> children; // Child offset in pages.bin
    Node* parent;

    Node();
    Node(int offset);
    ~Node();

    void writeToFile() const;
    void readFromFile();
    void print() const;

    bool isLeaf() const;
    bool isMin() const;
    bool isMax() const;
    bool canGive() const;
    bool canReceive() const;
    bool isOverflown() const;
    bool isUnderflown() const;
};
