#include "Block.h"

Block::Block(const std::array<Record, BLOCK_SIZE>& records) : records(records) { }

void Block::clear() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        records[i] = Record();
    }
}

void Block::print() const {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        records[i].print();
    }
}

void Block::writeToFile(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&records), sizeof(records));

    /*for (int i = 0; i < BLOCK_SIZE; i++) {
        records[i].writeToFile(filename);
    }*/
}

void Block::readFromFile(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&records), sizeof(records));

    for (int i = 0; i < BLOCK_SIZE; i++) {
        records[i].Product = records[i].product();
    }

    /*for (int i = 0; i < BLOCK_SIZE; i++) {
        records[i].readFromFile(filename);
    }*/
}
