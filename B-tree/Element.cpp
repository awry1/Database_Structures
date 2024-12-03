#include "Element.hpp"

Element::Element() : key(-1), offset(-1) {}

Element::Element(int key, int offset) : key(key), offset(offset) {}

void Element::writeToFile(std::fstream& file) const {
    file.write(reinterpret_cast<const char*>(&key), sizeof(key));
    file.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
}

void Element::readFromFile(std::fstream& file) {
    file.read(reinterpret_cast<char*>(&key), sizeof(key));
    file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
}

void Element::print() const {
    std::fstream file(RECORDS_FILE, std::ios::binary);
    file.seekg(offset);
    Record record;
    record.readFromFile(file);
    file.close();
    record.print();
}

void Element::update(Record record) const {
    std::fstream file(RECORDS_FILE, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(offset);
    record.writeToFile(file);
    file.close();
}
