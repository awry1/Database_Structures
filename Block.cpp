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

void Block::writeToFile(std::ofstream& file) {
    for (int i = 0; i < records.size(); i++) {
        records[i].writeToFile(file);
    }
    clear();
}

bool Block::readFromFile(std::ifstream& file) {
    // Return true if eof
    Record newRecord;
    newRecord.readFromFile(file);
    if (file.eof()) {
        return true;
    }
    this->records.push_back(newRecord);
    return false;
}
