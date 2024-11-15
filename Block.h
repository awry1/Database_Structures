#pragma once
#include "Record.h"
#define BLOCK_SIZE 4

class Block {
    public:
        std::vector<Record> records;
        std::ifstream *inFile;
        std::ofstream *outFile;
        int series;

        Block() = default;
        Block(std::vector<Record> &records);

        void print() const;
        void writeToFile(std::ofstream &file, int &counter);
        bool readFromFile(std::ifstream &file, int &counter);

        void clear();
        int size();
        bool isEmpty();
        bool isFull();
        Record first();
        Record pop();
};
