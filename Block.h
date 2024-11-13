#pragma once
#include "Record.h"
#define BLOCK_SIZE 4

class Block {
    public:
        std::array<Record, BLOCK_SIZE> records;

        Block() = default;
        Block(const std::array<Record, BLOCK_SIZE>& records);

        void clear();
        void print() const;
        void writeToFile(std::ofstream& file) const;
        void readFromFile(std::ifstream& file);
};
