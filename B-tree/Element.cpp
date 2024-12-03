#include "Element.hpp"

Element::Element() : key(-1), offset(-1) {}

Element::Element(int key, int offset) : key(key), offset(offset) {}

Record Element::readFromFile() const {
    std::ifstream file(RECORDS_FILE, std::ios::binary);
    file.seekg(offset);
    Record record;
    record.readFromFile(file);
    file.close();
    return record;
}

void Element::print() const {
    Record record = readFromFile();
    record.print();
}
