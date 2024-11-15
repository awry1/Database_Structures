#include "Block.h"
#define TAPE_A "tapeA.bin"
#define TAPE_B "tapeB.bin"
#define TAPE_C "tapeC.bin"

int Fibonacci(bool reset) {
    // Returns the next number in the Fibonacci sequence
    // 0, 1, 1, 2, 3, 5, 8, 13, ...
    static std::vector<int> fib = { 1, 0 };
    if (reset) {
        fib.clear();
        fib.push_back(1);
        fib.push_back(0);
        return -1;
    }
    fib.push_back(fib[fib.size() - 1] + fib[fib.size() - 2]);
    return fib[fib.size() - 2];
}

void inputRecords(std::string tapeName) {
    std::ofstream tape(tapeName, std::ios::binary);
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        record.readFromConsole();
        record.print();
        record.writeToFile(tape);
    }
    tape.close();
}

void generateRecords(std::string tapeName, int n) {
    std::ofstream tape(tapeName, std::ios::binary);
    for (int i = 0; i < n; i++) {
        Record record;
        record.randomize();
        record.writeToFile(tape);
    }
    tape.close();
    std::cout << n << " records written to " << tapeName << std::endl << std::endl;
}

void split(std::string tapeA, std::string tapeB, std::string tapeC, int &reads, int &writes, int &seriesA, int &seriesB) {
    std::ifstream inFile(tapeC, std::ios::binary);
    Block input;
    input.inFile = &inFile;

    std::ofstream outFileA(tapeA, std::ios::binary);
    Block outputA;
    outputA.outFile = &outFileA;

    std::ofstream outFileB(tapeB, std::ios::binary);
    Block outputB;
    outputB.outFile = &outFileB;

    Block* output = &outputA;

    int series = 1;
    bool eof = false;
    double last = -INFINITY;
    while (!eof) {
        eof = input.readFromFile(*input.inFile, reads);
        while (!input.isEmpty()) {
            Record record = input.pop();
            if (record.Product < last) {
                // End of series
                (*output).series++;
                if ((*output).series == series) {
                    series += Fibonacci(false);
                    output = (output == &outputA) ? &outputB : &outputA;
                }
                std::cout << "series -> " << "A:" << outputA.series << " | " << "B:" << outputB.series << std::endl;
                //std::cout << std::endl;
            }
            if ((*output).isFull()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            (*output).records.push_back(record);
            record.print();
            // TODO: Change to Block.last for better series recognition
            last = record.Product;
        }
        if (eof) {
            if (!(*output).isEmpty()) {
                (*output).series++;
                (*output).writeToFile(*(*output).outFile, writes);
                output = (output == &outputA) ? &outputB : &outputA;
            }
            if (!(*output).isEmpty()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
        }
    }
    std::cout << "final series -> " << "A:" << outputA.series << " | " << "B:" << outputB.series << std::endl;
    seriesA = outputA.series;
    seriesB = outputB.series;
    inFile.close();
    outFileA.close();
    outFileB.close();
}

void polyphaseMergeSort(std::string tapeA, std::string tapeB, std::string tapeC) {
    int readCounter = 0;
    int writeCounter = 0;
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;

    // Split tape C into A and B
    split(tapeA, tapeB, tapeC, readCounter, writeCounter, seriesA, seriesB);
    // Gaslight real number of series

    //while (true) {
    //    // Merge A and B into C
    //    // If A is empty, merge B and C into A
    //    // If B is empty, merge A and C into B
    //    // If C is empty, merge A and B into C
    //    // Repeat until 2 files are empty
    //}

    std::cout << std::endl << "Read counter: " << readCounter;
    std::cout << std::endl << "Write counter: " << writeCounter;
}

int main() {
    srand(unsigned int(time(NULL)));
    generateRecords(TAPE_C, 20);
    //inputRecords(TAPE_C);
    polyphaseMergeSort(TAPE_A, TAPE_B, TAPE_C);

    std::cout << std::endl;
    return 0;
}
