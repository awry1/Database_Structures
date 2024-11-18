#include "Block.h"
#include "Fibonacci.h"
#define TAPE_A "tapeA.bin"
#define TAPE_B "tapeB.bin"
#define TAPE_C "tapeC.bin"

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

    Fibonacci fib;

    int series = 1;
    bool eof = false;
    while (!eof) {
        eof = input.readFromFile(*input.inFile, reads);
        while (!input.isEmpty()) {
            Record record = input.pop();
            if (record.Product < (*output).last) {
                // End of series
                if ((*output).series == series) {
                    series += fib.next();
                    output = (output == &outputA) ? &outputB : &outputA;
                }
                if (record.Product < (*output).last) {
                    (*output).series++;
                }
                std::cout << "series -> " << "A:" << outputA.series << " | " << "B:" << outputB.series << std::endl;
                //std::cout << std::endl;
            }
            if ((*output).isFull()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            (*output).records.push_back(record);
            //record.print();
            (*output).last = record.Product;
        }
        if (eof) {
            if ((*output).isFull()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            output = (output == &outputA) ? &outputB : &outputA;
            if ((*output).isFull()) {
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

void correctSeries(int series1, int series2, int& newSeries1, int& newSeries2) {
    // calculate empty series in one tape, merge them at the beginning of merge fase
    Fibonacci fib;
    if (!(fib.isFib(series1) || fib.isFib(series2))) {
        std::cout << "something went wrong, both series are not Fibonacci numbers" << std::endl;
        return;
    }
    if (fib.isFib(series1) && fib.isFib(series2)) {
        newSeries1 = fib.next(newSeries1);
    }
    if (!fib.isFib(series1)) {
        newSeries1 = fib.closestFib(series1);
        if (newSeries1 == series2) {
            newSeries1 = fib.next(newSeries1);
        }
    }
    if (!fib.isFib(series2)) {
        newSeries2 = fib.closestFib(series2);
        if (newSeries2 == newSeries1) {
            newSeries2 = fib.next(newSeries2);
        }
    }
    std::cout << "corrected series -> " << newSeries1 << " | " << newSeries2 << std::endl;
}

void polyphaseMergeSort(std::string tapeA, std::string tapeB, std::string tapeC) {
    int readCounter = 0;
    int writeCounter = 0;
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;
    Fibonacci fib;

    // Split tape C into A and B
    split(tapeA, tapeB, tapeC, readCounter, writeCounter, seriesA, seriesB);
    // Gaslight the series numbers to make them fit the Fibonacci sequence
    int newSeriesA = seriesA;
    int newSeriesB = seriesB;
    correctSeries(seriesA, seriesB, newSeriesA, newSeriesB);
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
    generateRecords(TAPE_C, 47);
    //inputRecords(TAPE_C);
    polyphaseMergeSort(TAPE_A, TAPE_B, TAPE_C);

    std::cout << std::endl;
    return 0;
}
