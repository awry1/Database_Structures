#include "Btree.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"
#include "Node.hpp"
#include <iostream>
constexpr auto DISPLAY = false;
std::vector<int> existingKeys;
KeyGen keyGen(1000, true);

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void printRecordsFile(std::string fileName) {
    std::ifstream file(fileName, std::ios::binary);
    Record record;
    int records = 0;
    std::cout << std::endl << "Records" << std::endl;
    while (file.read(reinterpret_cast<char*>(&record.key), sizeof(record.key)) &&
        file.read(reinterpret_cast<char*>(&record.A), sizeof(record.A)) &&
        file.read(reinterpret_cast<char*>(&record.B), sizeof(record.B)) &&
        file.read(reinterpret_cast<char*>(&record.Sum), sizeof(record.Sum))) {
        record.print();
        records++;
    }
    file.close();
    std::cout << std::endl << "Records: " << records;
    std::cout << std::endl;
}

static void generateRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    for (int i = 0; i < n; i++) {
        Record record;
        record.randomize();
        record.key = keyGen.nextKey();
        btree.insertRecord(record);
        if (DISPLAY) {
            btree.printTree();
        }
        existingKeys.push_back(record.key);
    }
    std::cout << "Added " << n << " records to tree" << std::endl;
}

static void updateRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records to update: ";
    std::cin >> n;
    std::cin.ignore();
    if (n > existingKeys.size()) {
        std::cout << "Not enough records in tree" << std::endl;
        std::cout << "Updating all records in tree" << std::endl;
        n = existingKeys.size();
    }
    std::random_shuffle(existingKeys.begin(), existingKeys.end());
    for (int i = 0; i < n; i++) {
        Record record;
        record.randomize();
        record.key = existingKeys[i];
        btree.updateRecord(record);
    }
    std::cout << "Updated " << n << " records in tree" << std::endl;
}

static void deleteRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records to delete: ";
    std::cin >> n;
    std::cin.ignore();
    if (n > existingKeys.size()) {
        std::cout << "Not enough records in tree" << std::endl;
        std::cout << "Deleting all records from tree" << std::endl;
        n = existingKeys.size();
    }
    std::random_shuffle(existingKeys.begin(), existingKeys.end());
    for (int i = 0; i < n; i++) {
        //std::cout << "Deleting key " << existingKeys.back() << std::endl;
        btree.deleteRecord(existingKeys.back());
        if (DISPLAY) {
            btree.printTree();
        }
        existingKeys.pop_back();
    }
    std::cout << "Deleted " << n << " records from tree" << std::endl;
}

void printMetaMenu() {
    std::cout << std::endl;
    std::cout << "1. Generate records" << std::endl;
    std::cout << "2. Update records" << std::endl;
    std::cout << "3. Delete records" << std::endl;
    std::cout << "4. Clear files" << std::endl;
    std::cout << std::endl;
    std::cout << "5. Print records file" << std::endl;
    std::cout << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "-1. Back" << std::endl;
    std::cout << std::endl;
}

void metaCommands(Btree& btree) {
    int choice;
    while (true) {
        printMetaMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            generateRecords(btree);
            break;
        case 2:
            updateRecords(btree);
            break;
        case 3:
            deleteRecords(btree);
            break;
        case 4:
            btree.clearFiles();
            existingKeys.clear();
            std::cout << "Files cleared" << std::endl;
            break;
        case 5:
            printRecordsFile(RECORDS_FILE);
            break;
        case 0:
            exit(0);
            break;
        case -1:
            return;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
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
    std::cout << "0. Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "-1. Meta commands" << std::endl;
    std::cout << std::endl;
}

void programLoop() {
    Btree btree(DISPLAY);

    std::cout << "B-tree" << std::endl;
    int choice;
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
        case 0:
            btree.deleteTree();
            exit(0);
            break;
        case -1:
            metaCommands(btree);
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

int main() {
    //srand(unsigned int(time(NULL)));
    srand(42);

    programLoop();
}

// TODO:
// read and write counter
