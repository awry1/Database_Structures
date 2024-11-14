#include "Block.h"

Block::Block(std::vector<Record> &records) : records(records) {
    while (this->records.size() < BLOCK_SIZE) {
        this->records.push_back(Record());
    }
}

void Block::clear() {
    records.clear();
}

void Block::print() const {
    for (int i = 0; i < records.size(); i++) {
        records[i].print();
    }
}

void Block::writeToFile(std::ofstream &file, int &counter) {
    for (int i = 0; i < records.size(); i++) {
        records[i].writeToFile(file);
    }
    clear();
    counter++;
}

bool Block::readFromFile(std::ifstream &file, int &counter) {
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

int Block::size() {
    return records.size();
}

bool Block::empty() {
    return records.empty();
}

bool Block::full() {
    return (records.size() == BLOCK_SIZE);
}

Record Block::first() {
    return records.front();
}

Record Block::pop() {
    Record record = records.front();
    records.erase(records.begin());
    return record;
}
