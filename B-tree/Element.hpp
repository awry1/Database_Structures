#pragma once
#include <fstream>
#include <iostream>
#include "Record.hpp"
constexpr auto RECORDS_FILE = "records.bin";
constexpr auto RECORD_SIZE = sizeof(int) + 3 * sizeof(double);

class Element {
public:
    int key;
    int offset; // Record offset in records.bin

    Element();
    Element(int key, int offset);

    void writeToFile(std::fstream& file) const;
    void readFromFile(std::fstream& file);
    void print() const;

    void update(Record record) const;
};
