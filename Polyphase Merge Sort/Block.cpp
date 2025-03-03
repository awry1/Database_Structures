#include "Block.hpp"

Block::Block() {
    records = std::vector<Record>();
    inFile = nullptr;
    outFile = nullptr;
    last = INFINITY;
    series = 0;
}

Block::Block(std::vector<Record>& records) : records(records) {
    inFile = nullptr;
    outFile = nullptr;
    last = INFINITY;
    series = 0;
}

void Block::writeToFile(std::ofstream& file, int& counter) {
    if (records.empty()) {
        return;
    }

    for (int i = 0; i < records.size(); i++) {
        records[i].writeToFile(file);
    }
    clear();
    counter++;
}

bool Block::readFromFile(std::ifstream& file, int& counter) {
    // If the block is not empty, do not read from file
    if (!records.empty()) {
        return false;
    }

    bool eof = false;
    bool empty = true;
    while (records.size() < BLOCK_SIZE) {
        Record newRecord;
        newRecord.readFromFile(file);
        if (file.eof()) {
            eof = true;
            break;
        }
        this->records.push_back(newRecord);
        empty = false;
    }
    if (!empty) {
        counter++;
    }
    return eof;
}

void Block::print() const {
    for (int i = 0; i < records.size(); i++) {
        records[i].print();
    }
}

void Block::clear() {
    records.clear();
}

int Block::size() const {
    return int(records.size());
}

bool Block::isEmpty() const {
    return records.empty();
}

bool Block::isFull() const {
    return (records.size() == BLOCK_SIZE);
}

Record Block::first() {
    return records.front();
}

Record Block::pop() {
    Record record = first();
    records.erase(records.begin());
    return record;
}
