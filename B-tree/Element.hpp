#pragma once
#include <fstream>
#include <iostream>
#include "Record.hpp"
#include "Node.hpp"
constexpr auto RECORDS_FILE = "records.bin";
constexpr auto RECORD_SIZE = sizeof(int) + 3 * sizeof(double);

class Node;

class Element {
public:
    int key;
    int offset; // Record offset in records.bin
    Node* parent;

    Element();
    Element(int key, int offset);

    void writeToFile(std::fstream& file) const;
    void readFromFile(std::fstream& file);
    void print() const;

    void update(Record record) const;

    bool isNull() const;
};
