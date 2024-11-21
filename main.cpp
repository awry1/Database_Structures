#include "Block.h"
#include "Fibonacci.h"
#define TAPE_A "tapeA.bin"
#define TAPE_B "tapeB.bin"
#define TAPE_C "tapeC.bin"

void printTape(std::string tapeName, std::string displayName) {
    std::ifstream tape(tapeName, std::ios::binary);
    std::cout << std::endl << displayName << std::endl;
    int records = 0;
    int series = 1;
    float last = -INFINITY;
    while (true) {
        Record newRecord;
        newRecord.readFromFile(tape);
        if (tape.eof()) {
            break;
        }
        if (newRecord.Product < last) {
            //std::cout << std::endl;
            series++;
        }
        std::cout << ++records << ": ";
        newRecord.print();
        last = newRecord.Product;
    }
    std::cout << "Series: " << series << std::endl;
    tape.close();
}

void printTapes() {
    printTape(TAPE_A, "tape A");
    printTape(TAPE_B, "tape B");
    printTape(TAPE_C, "tape C");
}

void inputRecords(std::string tapeName) {
    std::ofstream tape(tapeName, std::ios::binary);
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    for (int i = 0; i < n; i++) {
        std::cout << "Record " << i + 1 << std::endl;
        record.readFromConsole();
        record.print();
        record.writeToFile(tape);
    }
    tape.close();
    std::cout << n << " records written to " << tapeName << std::endl;
}

void generateRecords(std::string tapeName) {
    std::ofstream tape(tapeName, std::ios::binary);
    Record record;
    int n;

    std::cout << "Enter number of records: ";
    std::cin >> n;
    std::cin.ignore();
    for (int i = 0; i < n; i++) {
        record.randomize();
        record.writeToFile(tape);
    }
    tape.close();
    std::cout << n << " records written to " << tapeName << std::endl;
}

void splitInputFile(std::string tapeA, std::string tapeB, std::string tapeC, int& reads, int& writes, int& seriesA, int& seriesB, bool display) {
    std::ifstream inFile(tapeC, std::ios::binary);
    Block input;
    input.inFile = &inFile;

    std::ofstream outFileA(tapeA, std::ios::binary);
    Block outputA;
    outputA.outFile = &outFileA;
    outputA.series = 1;

    std::ofstream outFileB(tapeB, std::ios::binary);
    Block outputB;
    outputB.outFile = &outFileB;
    outputB.series = 1;

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
            }
            if ((*output).isFull()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            (*output).records.push_back(record);
            (*output).last = record.Product;
        }
        if (eof) {
            if (!(*output).isEmpty()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
            output = (output == &outputA) ? &outputB : &outputA;
            if (!(*output).isEmpty()) {
                (*output).writeToFile(*(*output).outFile, writes);
            }
        }
    }
    seriesA = outputA.series;
    seriesB = outputB.series;
    inFile.close();
    outFileA.close();
    outFileB.close();

    if (display) {
        std::cout << std::endl << "------ After splitting ------";
        printTape(tapeA, "tape A");
        printTape(tapeB, "tape B");
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
}

void calculateDummySeries(int series1, int series2, int& dummies1, int& dummies2) {
    Fibonacci fib;
    if (!(fib.isFib(series1) || fib.isFib(series2))) {
        std::cout << std::endl << "Both series are not Fibonacci numbers" << std::endl;
        exit(1);
    }
    if (fib.isFib(series1) && fib.isFib(series2)) {
        dummies2 = fib.next(series2) - series2;
        return;
    }
    if (!fib.isFib(series1)) {
        int nextFib = fib.closestFib(series1);
        if (nextFib == series2) {
            nextFib = fib.next(nextFib);
        }
        dummies1 = nextFib - series1;
        return;
    }
    if (!fib.isFib(series2)) {
        int nextFib = fib.closestFib(series2);
        if (nextFib == series1) {
            nextFib = fib.next(nextFib);
        }
        dummies2 = nextFib - series2;
        return;
    }
}

void handleDummySeries(Block& input1, Block& input2, Block& output, int& reads, int& writes, int& series1, int& series2, int& seriesOut, int& dummies1, int& dummies2) {
    Block* input = (dummies1 > 0) ? &input2 : &input1;
    int* series = (dummies1 > 0) ? &series2 : &series1;
    int* dummies = (dummies1 > 0) ? &dummies1 : &dummies2;

    Fibonacci fib;

    while (*dummies > 0) {
        input->readFromFile(*input->inFile, reads);
        while (!input->isEmpty()) {
            Record record = input->pop();
            if (record.Product < output.last) {
                // End of series
                seriesOut++;
                *series -= 1;
                *dummies -= 1;
                if (*dummies == 0) {
                    // Put back the record
                    input->records.insert(input->records.begin(), record);
                    break;
                }
            }
            if (output.isFull()) {
                output.writeToFile(*output.outFile, writes);
            }
            output.records.push_back(record);
            output.last = record.Product;
        }
        if (*dummies == 0) {
            if (!output.isEmpty()) {
                output.writeToFile(*output.outFile, writes);
            }
        }
    }
}

bool mergeFiles(std::string& tapeIn1, std::string& tapeIn2, std::string& tapeOut, int& reads, int& writes, int& seriesIn1, int& seriesIn2, int& seriesOut, bool display) {
    std::ifstream inFile1(tapeIn1, std::ios::binary);
    Block input1;
    input1.inFile = &inFile1;

    std::ifstream inFile2(tapeIn2, std::ios::binary);
    Block input2;
    input2.inFile = &inFile2;

    std::ofstream outFile(tapeOut, std::ios::binary);
    Block output;
    output.outFile = &outFile;

    Fibonacci fib;

    int dummies1 = 0;
    int dummies2 = 0;
    calculateDummySeries(seriesIn1, seriesIn2, dummies1, dummies2);
    handleDummySeries(input1, input2, output, reads, writes, seriesIn1, seriesIn2, seriesOut, dummies1, dummies2);

    inFile1.close();
    inFile2.close();
    outFile.close();

    if (display) {
        std::cout << std::endl << "------ After dummies --------";
        printTapes();
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }

    std::cout << "seriesIn1: " << seriesIn1 << std::endl;
    std::cout << "seriesIn2: " << seriesIn2 << std::endl;
    std::cout << "seriesOut: " << seriesOut << std::endl;
    exit(0);

    // Update tapes
    std::string tmp;
    if (seriesIn1 == 0) {
        tmp = tapeIn1;
        tapeIn1 = tapeOut;
        tapeOut = tmp;
    }
    else if (seriesIn2 == 0) {
        tmp = tapeIn2;
        tapeIn2 = tapeOut;
        tapeOut = tmp;
    }
    else {
        std::cout << std::endl << "No empty tape after merging" << std::endl;
        //exit(1);
    }
    return true;
}

void polyphaseMergeSort(std::string tapeA, std::string tapeB, std::string tapeC, bool display) {
    int reads = 0;
    int writes = 0;
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;

    if (display) {
        printTape(tapeC, "Input file");
    }

    // Split tape C into A and B
    splitInputFile(tapeA, tapeB, tapeC, reads, writes, seriesA, seriesB, display);

    std::string tapeIn1 = tapeA;
    std::string tapeIn2 = tapeB;
    std::string tapeOut = tapeC;
    bool sorted = false;
    while (!sorted) {
        // Merge 2 non-empty tapes
        sorted = mergeFiles(tapeIn1, tapeIn2, tapeOut, reads, writes, seriesA, seriesB, seriesC, display);
    }

    std::cout << std::endl << "------ After sorting --------";
    printTape(tapeOut, "Sorted file");
    std::cout << std::endl << "Read counter: " << reads;
    std::cout << std::endl << "Write counter: " << writes;
    std::cout << std::endl;
}

int main() {
    srand(unsigned int(time(NULL)));
    // TODO: Implement menu
    bool autoGenerate = true;
    bool display = true;
    if (autoGenerate) {
        generateRecords(TAPE_C);
    }
    else {
        inputRecords(TAPE_C);
    }
    polyphaseMergeSort(TAPE_A, TAPE_B, TAPE_C, display);
    return 0;
}
