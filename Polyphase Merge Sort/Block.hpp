#pragma once
#include <fstream>
#include <vector>
#include "Record.hpp"
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

    void writeToFile(std::ofstream& file, int& counter);
    bool readFromFile(std::ifstream& file, int& counter);
    void print() const;

    void clear();
    int size() const;
    bool isEmpty() const;
    bool isFull() const;

    Record first();
    Record pop();
};
