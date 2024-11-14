#include "Block.h"

void split(std::string a, std::string b, std::string c, int &readCounter, int &writeCounter, int &seriesA, int &seriesB) {
    std::ofstream A(a, std::ios::binary);
    std::ofstream B(b, std::ios::binary);
    std::ifstream C(c, std::ios::binary);
    std::ofstream* outFile = &A;
    std::ifstream* inFile = &C;

    int* series = &seriesA;

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
                // switch output following fibonacci series
                // TODO
                (*series)++;
                if (!output.empty()) {
                    output.writeToFile((*outFile), writeCounter);
                }
                if (outFile == &A) {
                    outFile = &B;
                    series = &seriesB;
                    std::cout << std::endl;
                }
                else {
                    outFile = &A;
                    series = &seriesA;
                    std::cout << std::endl;
                }
            }
            last = record.Product;
            std::cout << record.Product << ": ";
            record.print();
            if (output.full()) {
                output.writeToFile((*outFile), writeCounter);
            }
            output.records.push_back(record);
        }
        if (eof && !output.empty()) {
            (*series)++;
            output.writeToFile((*outFile), writeCounter);
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
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;
    std::string A = "A.bin";
    std::string B = "B.bin";
    std::string C = "C.bin";

    split(A, B, C, readCounter, writeCounter, seriesA, seriesB);
    // gaslight real number of series

    std::cout << std::endl << "Read counter: " << readCounter;
    std::cout << std::endl << "Write counter: " << writeCounter;
    std::cout << std::endl << "Series A: " << seriesA;
    std::cout << std::endl << "Series B: " << seriesB;
    

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
    for (int i = 0; i < 5; i++) {
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
