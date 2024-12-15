#include "Btree.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"
#include "Node.hpp"
#include <iostream>
constexpr auto DISPLAY = true;
std::vector<int> existingKeys;
KeyGen keyGen(1000, true);

static void clearConsole() {
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

static void generateRecords(Btree& btree, int n) {
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
    int reads = btree.reads / n;
    int writes = btree.writes / n;
    btree.reads = 0;
    btree.writes = 0;
    std::ofstream file("results.txt", std::ios::app);
    file << "inserts: " << n << " | av reads: " << reads << " | av writes: " << writes << std::endl;
    file.close();
}

static void updateRecords(Btree& btree, int n) {
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
    int reads = btree.reads / n;
    int writes = btree.writes / n;
    btree.reads = 0;
    btree.writes = 0;
    std::ofstream file("results.txt", std::ios::app);
    file << "updates: " << n << " | av reads: " << reads << " | av writes: " << writes << std::endl;
    file.close();
}

static void deleteRecords(Btree& btree, int n) {
    if (n > existingKeys.size()) {
        std::cout << "Not enough records in tree" << std::endl;
        std::cout << "Deleting all records from tree" << std::endl;
        n = existingKeys.size();
    }
    std::random_shuffle(existingKeys.begin(), existingKeys.end());
    for (int i = 0; i < n; i++) {
        btree.deleteRecord(existingKeys.back());
        if (DISPLAY) {
            btree.printTree();
        }
        existingKeys.pop_back();
    }
    std::cout << "Deleted " << n << " records from tree" << std::endl;
    int reads = btree.reads / n;
    int writes = btree.writes / n;
    btree.reads = 0;
    btree.writes = 0;
    std::ofstream file("results.txt", std::ios::app);
    file << "deletes: " << n << " | av reads: " << reads << " | av writes: " << writes << std::endl;
    file.close();
}

static void experiment(Btree& btree) {
    int i, u, d;
    std::ifstream file("experiment.txt");
    if (!file.is_open()) {
        std::cout << "File experiment.txt not found, creating new file (100, 50, 75)" << std::endl;
        std::ofstream file("experiment.txt");
        file << 100 << " " << 50 << " " << 75 << std::endl;
        file.close();
        i = 100;
        u = 50;
        d = 75;
    }
    else {
        file >> i >> u >> d;
        file.close();
    }

    btree.clearFiles();
    generateRecords(btree, i);
    updateRecords(btree, u);
    deleteRecords(btree, d);
    btree.clearFiles();

    std::cout << "Experiment completed" << std::endl;
}

static void userGenerateRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    generateRecords(btree, n);
}

static void userUpdateRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records to update: ";
    std::cin >> n;
    std::cin.ignore();
    updateRecords(btree, n);
}

static void userDeleteRecords(Btree& btree) {
    int n;
    std::cout << "Enter number of records to delete: ";
    std::cin >> n;
    std::cin.ignore();
    deleteRecords(btree, n);
}

static void printMoreMenu() {
    std::cout << std::endl;
    std::cout << "1. Generate records" << std::endl;
    std::cout << "2. Update records" << std::endl;
    std::cout << "3. Delete records" << std::endl;
    std::cout << "4. Clear files" << std::endl;
    std::cout << std::endl;
    std::cout << "5. Print records file" << std::endl;
    std::cout << "6. Execute saved instuctions" << std::endl;
    std::cout << std::endl;
    std::cout << "9. Back" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << std::endl;
}

static void moreCommands(Btree& btree) {
    int choice;
    while (true) {
        printMoreMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            userGenerateRecords(btree);
            break;
        case 2:
            userUpdateRecords(btree);
            break;
        case 3:
            userDeleteRecords(btree);
            break;
        case 4:
            btree.clearFiles();
            existingKeys.clear();
            std::cout << "Files cleared" << std::endl;
            break;
        case 5:
            printRecordsFile(RECORDS_FILE);
            break;
        case 6:
            experiment(btree);
            break;
        case 9:
            return;
            break;
        case 0:
            btree.deleteTree();
            exit(0);
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

static void printMenu() {
    std::cout << std::endl;
    std::cout << "1. Insert record" << std::endl;
    std::cout << "2. Update record" << std::endl;
    std::cout << "3. Delete record" << std::endl;
    std::cout << std::endl;
    std::cout << "4. Print record" << std::endl;
    std::cout << "5. Print tree" << std::endl;
    std::cout << "6. Print all" << std::endl;
    std::cout << std::endl;
    std::cout << "9. More commands" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << std::endl;
}

static void programLoop() {
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
        case 9:
            moreCommands(btree);
            break;
        case 0:
            btree.deleteTree();
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

    programLoop();
}
