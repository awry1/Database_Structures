#include "Btree.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"
#include "Node.hpp"
#include <iostream>

static void getRecordsFileInfo(std::string fileName, int& records) {
    std::ifstream file(fileName, std::ios::binary);
    Record record;
    records = 0;
    while (file.read(reinterpret_cast<char*>(&record.key), sizeof(record.key)) &&
        file.read(reinterpret_cast<char*>(&record.A), sizeof(record.A)) &&
        file.read(reinterpret_cast<char*>(&record.B), sizeof(record.B)) &&
        file.read(reinterpret_cast<char*>(&record.Sum), sizeof(record.Sum))) {
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

static void printRecordsFile(std::string fileName) {
    std::ifstream file(fileName, std::ios::binary);
    Record record;
    std::cout << std::endl << "Records" << std::endl;
    while (file.read(reinterpret_cast<char*>(&record.key), sizeof(record.key)) &&
        file.read(reinterpret_cast<char*>(&record.A), sizeof(record.A)) &&
        file.read(reinterpret_cast<char*>(&record.B), sizeof(record.B)) &&
        file.read(reinterpret_cast<char*>(&record.Sum), sizeof(record.Sum))) {
        record.print();
    }
    file.close();
}

static void generateRecords(Btree& btree) {
    btree.clearFiles();
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    KeyGen keyGen(n, true);
    for (int i = 0; i < n; i++) {
        record.randomize();
        record.key = keyGen.nextKey();
        btree.insertRecord(record);
    }
    std::cout << "Added " << n << " records to tree" << std::endl;
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printMenu() {
    std::cout << std::endl;
    std::cout << "1. Insert record" << std::endl;
    std::cout << "2. Update record" << std::endl;
    std::cout << "3. Delete record" << std::endl;
    std::cout << std::endl;
    std::cout << "4. Print record" << std::endl;
    std::cout << "5. Print tree" << std::endl;
    std::cout << "6. Print all" << std::endl;
    std::cout << std::endl;
    std::cout << "9. Clear files" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << std::endl;
}

void programLoop() {
    Btree btree;

    int choice;
    std::cout << "B-tree" << std::endl;
    std::cout << "1. Load tree" << std::endl;
    std::cout << "2. Empty tree" << std::endl;
    std::cout << "3. Populate tree" << std::endl;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    std::cin.ignore();
    if (choice == 1) {
        std::cout << "Tree loaded" << std::endl;
    }
    else if (choice == 2) {
        btree.clearFiles();
    }
    else if (choice == 3) {
        generateRecords(btree);
    }
    else {
        std::cout << "Invalid choice" << std::endl;
        exit(1);
    }

    while (true) {
        printMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            btree.insertRecord();
            break;
        case 2:
            btree.updateRecord();
            break;
        case 3:
            btree.deleteRecord();
            break;
        case 4:
            btree.printRecord();
            break;
        case 5:
            btree.printTree();
            break;
        case 6:
            btree.printAllRecords();
            break;
        case 9:
            btree.clearFiles();
            std::cout << "Populate tree? (0/1): ";
            std::cin >> choice;
            std::cin.ignore();
            if (choice == 1) {
                generateRecords(btree);
            }
            break;
        case 0:
            btree.deleteTree();
            exit(0);
            break;
        case -1:
            int records;
            getRecordsFileInfo(RECORDS_FILE, records);
            std::cout << "Biggest key: " << records - 1 << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

int main() {
    srand(unsigned int(time(NULL)));

    programLoop();
}
