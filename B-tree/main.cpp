#include "Btree.hpp"
#include "Element.hpp"
#include "KeyGen.hpp"
#include "Node.hpp"
#include <iostream>

//static void getInputInfo(std::string fileName, int& records) {
//    std::fstream file(fileName, std::ios::binary);
//    Record newRecord;
//    records = 0;
//    while (true) {
//        newRecord.readFromFile(file);
//        if (file.eof()) {
//            break;
//        }
//        records++;
//    }
//    file.close();
//    if (records == 0) {
//        std::cout << std::endl << "Empty input file" << std::endl;
//        exit(1);
//    }
//    std::cout << std::endl << "Records: " << records;
//    std::cout << std::endl;
//}

//static void printFile(std::string fileName, std::string displayName) {
//    std::fstream file(fileName, std::ios::binary);
//    std::cout << std::endl << displayName << std::endl;
//
//    int records = 0;
//    bool empty = true;
//    while (true) {
//        Record newRecord;
//        newRecord.readFromFile(file);
//        if (file.eof()) {
//            if (!empty) {
//                std::cout << "Records: " << records << std::endl;
//            }
//            else {
//                std::cout << "Empty file" << std::endl;
//            }
//            break;
//        }
//        records++;
//        //std::cout << records << ": ";
//        newRecord.print();
//        empty = false;
//    }
//    file.close();
//}

//static void generateRecords(std::string fileName) {
//    std::fstream file(fileName, std::ios::binary);
//    Record record;
//    int n;
//
//    std::cout << "Enter number of records: ";
//    std::cin >> n;
//    std::cin.ignore();
//    KeyGen keyGen(n);
//    for (int i = 0; i < n; i++) {
//        record.randomize();
//        record.key = keyGen.nextKey();
//        record.writeToFile(file);
//    }
//    file.close();
//    std::cout << n << " records written to " << fileName << std::endl;
//}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printMenu() {
    std::cout << std::endl;
    std::cout << "1. Find record" << std::endl;
    std::cout << "2. Insert record" << std::endl;
    std::cout << "3. Update record" << std::endl;
    std::cout << "4. Delete record" << std::endl;
    std::cout << "5. Print tree structure" << std::endl;
    std::cout << "6. Print records in order" << std::endl;
    std::cout << "0. Exit" << std::endl;
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
            btree.printInOrder();
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

void examplePrint() {

    Record record0(0, 0.5, 0.2, 0.3);
    Element element0(0, 0 * sizeof(Record));
    element0.update(record0);

    Record record1(1, 0.5, 0.2, 0.3);
    Element element1(1, 1 * sizeof(Record));
    element1.update(record1);

    Record record2(2, 0.5, 0.2, 0.3);
    Element element2(2, 2 * sizeof(Record));
    element2.update(record2);

    Record record3(3, 0.5, 0.2, 0.3);
    Element element3(3, 3 * sizeof(Record));
    element3.update(record3);

    Record record4(4, 0.5, 0.2, 0.3);
    Element element4(4, 4 * sizeof(Record));
    element4.update(record4);

    Record record5(5, 0.5, 0.2, 0.3);
    Element element5(5, 5 * sizeof(Record));
    element5.update(record5);

    Record record6(6, 0.5, 0.2, 0.3);
    Element element6(6, 6 * sizeof(Record));
    element6.update(record6);

    Btree btree;
    btree.root->elements.push_back(element3);
    btree.root->children.push_back(1 * sizeof(Node));
    btree.root->children.push_back(2 * sizeof(Node));
    btree.root->writeToFile();

    Node child1(1 * sizeof(Node));
    child1.elements.push_back(element1);
    child1.children.push_back(3 * sizeof(Node));
    child1.children.push_back(4 * sizeof(Node));
    child1.writeToFile();

    Node child2(2 * sizeof(Node));
    child2.elements.push_back(element5);
    child2.children.push_back(5 * sizeof(Node));
    child2.children.push_back(6 * sizeof(Node));
    child2.writeToFile();

    Node child3(3 * sizeof(Node));
    child3.elements.push_back(element0);
    child3.writeToFile();

    Node child4(4 * sizeof(Node));
    child4.elements.push_back(element2);
    child4.writeToFile();

    Node child5(5 * sizeof(Node));
    child5.elements.push_back(element4);
    child5.writeToFile();

    Node child6(6 * sizeof(Node));
    child6.elements.push_back(element6);
    child6.writeToFile();

    btree.printTree();
    btree.printTreeReverse();
    btree.deleteTree();
}

int main() {
    srand(unsigned int(time(NULL)));

    //programLoop();

    examplePrint();
}
