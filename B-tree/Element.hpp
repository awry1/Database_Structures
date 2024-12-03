#pragma once
#include <fstream>
#include <iostream>
#include "Record.hpp"
constexpr auto RECORDS_FILE = "records.bin";
constexpr auto RECORD_SIZE = sizeof(int) + 3 * sizeof(double);

class Element {
public:
    int key;
    int offset; // offset of the record in the file

    Element();
    Element(int key, int offset);

    Record readFromFile() const;

    void print() const;
};
