#include <iostream>
#include "Btree.hpp"
#include "Node.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"

static void getInputInfo(std::string fileName, int& records) {
    std::fstream file(fileName, std::ios::binary);
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
    std::fstream file(fileName, std::ios::binary);
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

static void generateRecords(std::string fileName) {
    std::fstream file(fileName, std::ios::binary);
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

void printMenu() {
    std::cout << std::endl;
    std::cout << "1. Find record" << std::endl;
    std::cout << "2. Insert record" << std::endl;
    std::cout << "3. Update record" << std::endl;
    std::cout << "4. Delete record" << std::endl;
    std::cout << "5. Print tree" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << std::endl;
}

void programLoop() {
    Btree btree;

    int choice;
    while (true) {
        printMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            btree.findRecord();
            break;
        case 2:
            btree.insertRecord();
            break;
        case 3:
            btree.updateRecord();
            break;
        case 4:
            btree.deleteRecord();
            break;
        case 5:
            btree.printTree();
            break;
        case 6:
            exit(0);
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

int main() {
    srand(unsigned int(time(NULL)));

    //programLoop();

    Btree btree;
}
