#include "Block.h"

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

void split(std::string inputName, int &reads, int &writes, int &seriesA, int &seriesB) {
    std::ifstream inFile(inputName, std::ios::binary);
    Block input;
    input.inFile = &inFile;

    std::ofstream outFileA("tapeA.bin", std::ios::binary);
    Block outputA;
    outputA.outFile = &outFileA;

    std::ofstream outFileB("tapeB.bin", std::ios::binary);
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
            }
            if ((*output).isFull()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            (*output).records.push_back(record);
            std::cout << record.Product << " -> ";
            record.print();
            last = record.Product;
        }
        if (eof) {
            if (!outputA.isEmpty()) {
                outputA.series++;
                outputA.writeToFile(*outputA.outFile, writes);
            }
            if (!outputB.isEmpty()) {
                outputB.series++;
                outputB.writeToFile(*outputB.outFile, writes);
            }
        }
    }
    seriesA = outputA.series;
    seriesB = outputB.series;
    inFile.close();
    outFileA.close();
    outFileB.close();
}

void polyphaseMergeSort(std::string tapeName) {
    int readCounter = 0;
    int writeCounter = 0;
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;

    split(tapeName, readCounter, writeCounter, seriesA, seriesB);
    std::cout << std::endl << "Series A: " << seriesA;
    std::cout << std::endl << "Series B: " << seriesB;
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
    std::string input = "tapeC.bin";
    generateRecords(input, 20);
    polyphaseMergeSort(input);

    std::cout << std::endl;
    return 0;
}
