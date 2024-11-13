#pragma once
#include "Record.h"
#define BLOCK_SIZE 4

class Block {
    public:
        std::vector<Record> records;

        Block() = default;
        Block(std::vector<Record>& records);

        void clear();
        void print() const;
        void writeToFile(std::ofstream& file);
        bool readFromFile(std::ifstream& file);
};
