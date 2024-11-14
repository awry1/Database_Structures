#include "Block.h"

void split(std::string a, std::string b, std::string c, int &readCounter, int &writeCounter) {
    std::ofstream A(a, std::ios::binary);
    std::ofstream B(b, std::ios::binary);
    std::ifstream C(c, std::ios::binary);
    std::ofstream* outFile = &A;
    std::ifstream* inFile = &C;

    Block input;
    Block output;

    // Split C (input) into A and B (output)
    bool eof = false;
    double last = -INFINITY;
    while (!eof) {
        eof = input.readFromFile((*inFile), readCounter);
        while (!input.empty()) {
            Record record = input.pop();
            if (record.Product < last) {
                // Switch output file
                std::cout << std::endl;
                if (outFile == &A) {
                    outFile = &B;
                }
                else {
                    outFile = &A;
                }
            }
            last = record.Product;
            std::cout << record.Product << ": ";
            record.print();
            if (output.full()) {
                output.writeToFile((*outFile), writeCounter); /// <--- ERROR
            }
            output.records.push_back(record);
        }
        if (eof && !output.empty()) {
            output.writeToFile((*outFile), writeCounter); /// <--- ERROR
            input.clear();
        }
    }
    A.close();
    B.close();
    C.close();
}

void polyphaseMergeSort() {
    int readCounter = 0;
    int writeCounter = 0;
    std::string A = "A.bin";
    std::string B = "B.bin";
    std::string C = "C.bin";

    split(A, B, C, readCounter, writeCounter);

    std::cout << std::endl << "Read counter: " << readCounter;
    std::cout << std::endl << "Write counter: " << writeCounter;
    

    //while (true) {
    //    // Merge A and B into C
    //    // If A is empty, merge B and C into A
    //    // If B is empty, merge A and C into B
    //    // If C is empty, merge A and B into C
    //    // Repeat until 2 files are empty
    //}
}

int main() {
    srand(unsigned int(time(NULL)));

    // Generate random records
    std::ofstream C("C.bin", std::ios::binary);
    for (int i = 0; i < 24; i++) {
        Record record;
        record.randomize();
        record.print();
        record.writeToFile(C);
    }
    C.close();
    std::cout << std::endl;

    polyphaseMergeSort();

    std::cout << std::endl;
    return 0;
}
