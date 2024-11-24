#pragma once
#include "Record.h"
constexpr auto BLOCK_SIZE = 100;

class Block {
public:
    std::vector<Record> records;
    std::ifstream* inFile;
    std::ofstream* outFile;
    double last;
    int series;

    Block();
    Block(std::vector<Record>& records);

    void print() const;
    void writeToFile(std::ofstream& file, int& counter);
    bool readFromFile(std::ifstream& file, int& counter);

    void clear();
    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    Record first();
    Record pop();
};
