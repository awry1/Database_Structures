#pragma once
#include "Record.h"
#define BLOCK_SIZE 4

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
    bool isEmpty();
    bool isFull();
    int size();
    Record first();
    Record pop();
};
