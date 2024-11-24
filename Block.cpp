#include "Block.h"

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
    while (this->records.size() < BLOCK_SIZE) {
        this->records.push_back(Record());
    }
}

void Block::print() const {
    for (int i = 0; i < records.size(); i++) {
        records[i].print();
    }
}

void Block::writeToFile(std::ofstream& file, int& counter) {
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

void Block::clear() {
    records.clear();
}

bool Block::isEmpty() {
    return records.empty();
}

bool Block::isFull() {
    return (records.size() == BLOCK_SIZE);
}

int Block::size() {
    return int(records.size());
}

Record Block::first() {
    return records.front();
}

Record Block::pop() {
    Record record = first();
    records.erase(records.begin());
    return record;
}
