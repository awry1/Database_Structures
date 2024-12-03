#include <iostream>
#include "Btree.hpp"
#include "Node.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"

static void getInputInfo(std::string fileName, int& records) {
    std::ifstream file(fileName, std::ios::binary);
    Record newRecord;
    records = 0;
    while (true) {
        newRecord.readFromFile(file);
        if (file.eof()) {
            break;
        }
        records++;
    }
    file.close();
    if (records == 0) {
        std::cout << std::endl << "Empty input file" << std::endl;
        exit(1);
    }
    std::cout << std::endl << "Records: " << records;
    std::cout << std::endl;
}

static void printFile(std::string fileName, std::string displayName) {
    std::ifstream file(fileName, std::ios::binary);
    std::cout << std::endl << displayName << std::endl;

    int records = 0;
    bool empty = true;
    while (true) {
        Record newRecord;
        newRecord.readFromFile(file);
        if (file.eof()) {
            if (!empty) {
                std::cout << "Records: " << records << std::endl;
            }
            else {
                std::cout << "Empty file" << std::endl;
            }
            break;
        }
        records++;
        //std::cout << records << ": ";
        newRecord.print();
        empty = false;
    }
    file.close();
}

static void inputRecords(std::string fileName) {
    // TODO: add creation of elements while generating records
    std::ofstream file(fileName, std::ios::binary);
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    for (int i = 0; i < n; i++) {
        std::cout << "Record " << i + 1 << std::endl;
        record.readFromConsole();
        record.print();
        record.writeToFile(file);
    }
    file.close();
    std::cout << n << " records written to " << fileName << std::endl;
}

static void generateRecords(std::string fileName) {
    // TODO: add creation of elements while generating records
    std::ofstream file(fileName, std::ios::binary);
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    KeyGen keyGen(n);
    for (int i = 0; i < n; i++) {
        record.randomize();
        record.key = keyGen.nextKey();
        record.writeToFile(file);
    }
    file.close();
    std::cout << n << " records written to " << fileName << std::endl;
}

int main() {
    srand(unsigned int(time(NULL)));

    generateRecords("records.bin");
    int records = 0;
    getInputInfo("records.bin", records);
    int offset = 0;
    for (int i = 0; i < records; i++) {
        Element element(i, offset);
        element.print();
        offset += RECORD_SIZE;
    }
}
