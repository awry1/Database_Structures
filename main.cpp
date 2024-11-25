#include "Block.h"
#include "Fibonacci.h"
constexpr auto TAPE_A = "tapeA.bin";
constexpr auto TAPE_B = "tapeB.bin";
constexpr auto TAPE_C = "tapeC.bin";

static void getInputInfo(std::string tapeName, int& series, int& records) {
    std::ifstream tape(tapeName, std::ios::binary);
    Record newRecord;
    series = 0;
    records = 0;
    double last = INFINITY;
    while (true) {
        newRecord.readFromFile(tape);
        if (tape.eof()) {
            break;
        }
        if (newRecord.Product < last) {
            series++;
        }
        records++;
        last = newRecord.Product;
    }
    tape.close();
    if (records == 0) {
        std::cout << std::endl << "Empty input file" << std::endl;
        exit(1);
    }
    std::cout << std::endl << "Records: " << records;
    std::cout << std::endl << "Series: " << series;
    std::cout << std::endl << "Block: " << BLOCK_SIZE << " records";
    std::cout << std::endl;
}

static void printTape(std::string tapeName, std::string displayName) {
    std::ifstream tape(tapeName, std::ios::binary);
    std::cout << std::endl << displayName << std::endl;

    int records = 0;
    int series = 0;
    double last = INFINITY;
    bool empty = true;
    while (true) {
        Record newRecord;
        newRecord.readFromFile(tape);
        if (tape.eof()) {
            if (!empty) {
                std::cout << "Records: " << records << std::endl;
                std::cout << "Series: " << series << std::endl;
            }
            else {
                std::cout << "Empty tape" << std::endl;
            }
            break;
        }
        if (newRecord.Product < last) {
            //std::cout << std::endl;
            series++;
        }
        records++;
        //std::cout << records << ": ";
        newRecord.print();
        last = newRecord.Product;
        empty = false;
    }
    tape.close();
}

static void printTapes() {
    printTape(TAPE_A, "tape A");
    printTape(TAPE_B, "tape B");
    printTape(TAPE_C, "tape C");
}

static void inputRecords(std::string tapeName) {
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

static void generateRecords(std::string tapeName) {
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

static void splitInputFile(std::string tapeA, std::string tapeB, std::string tapeC, int& seriesA, int& seriesB, int& reads, int& writes, bool display) {
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
    while (true) {
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
            break;
        }
    }
    inFile.close();
    outFileA.close();
    outFileB.close();

    seriesA = outputA.series;
    seriesB = outputB.series;

    if (seriesA == 0 && seriesB == 0) {
        std::cout << std::endl << "Empty input file" << std::endl;
        exit(1);
    }

    if (display) {
        std::cout << std::endl << "------ After splitting ------";
        printTape(tapeA, "tape A");
        printTape(tapeB, "tape B");
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
}

static void calculateDummySeries(int series1, int series2, int& dummies1, int& dummies2) {
    static bool first = true;
    if (!first) {
        return;
    }
    first = false;

    Fibonacci fib;

    if (!fib.isFib(series1) && !fib.isFib(series2)) {
        std::cout << std::endl << "Both series are not Fibonacci numbers" << std::endl;
        exit(1);
    }
    if (fib.isFib(series1) && fib.isFib(series2)) {
        if (series1 == series2) {
            if (series1 == 1) {
                return;
            }
            dummies2 = fib.next(series2) - series2;
            return;
        }
        int smaller = std::min(series1, series2);
        int larger = std::max(series1, series2);
        int nextFib = fib.next(smaller);
        if (nextFib == larger) {
            return;
        }
        if (nextFib == smaller) {
            return;
        }
        std::cout << std::endl << "Series are Fibonacci numbers but not consecutive" << std::endl;
    }
    if (!fib.isFib(series1)) {
        if (series1 > fib.next(series2)) {
            std::cout << std::endl << "Series 1 is too large" << std::endl;
            exit(1);
        }
        if (series1 < fib.next(series2) - series2) {
            std::cout << std::endl << "Series 1 is too small" << std::endl;
            exit(1);
        }
        int nextFib = fib.next(series1);
        if (nextFib == series2) {
            nextFib = fib.next(nextFib);
        }
        dummies1 = nextFib - series1;
        return;
    }
    if (!fib.isFib(series2)) {
        if (series2 > fib.next(series1)) {
            std::cout << std::endl << "Series 2 is too large" << std::endl;
            exit(1);
        }
        if (series2 < fib.next(series1) - series1) {
            std::cout << std::endl << "Series 2 is too small" << std::endl;
            exit(1);
        }
        int nextFib = fib.closestFib(series2);
        if (nextFib == series1) {
            nextFib = fib.next(nextFib);
        }
        dummies2 = nextFib - series2;
        return;
    }
}

static void handleDummySeries(Block& input1, Block& input2, Block& output, int& series1, int& series2, int& seriesOut, int& dummies1, int& dummies2, int& reads, int& writes) {
    if (dummies1 == 0 && dummies2 == 0) {
        output.last = INFINITY;
        return;
    }

    Block* input = (dummies1 > 0) ? &input2 : &input1;
    int* series = (dummies1 > 0) ? &series2 : &series1;
    int* dummies = (dummies1 > 0) ? &dummies1 : &dummies2;

    bool eof = false;
    while (true) {
        eof = input->readFromFile(*input->inFile, reads);
        while (!input->isEmpty()) {
            Record record = input->first();
            if (record.Product < output.last) {
                // End of series
                seriesOut++;
                *series -= 1;
                *dummies -= 1;
                if (*dummies == 0) {
                    output.last = INFINITY;
                    return;
                }
            }
            if (output.isFull()) {
                output.writeToFile(*output.outFile, writes);
            }
            output.records.push_back(input->pop());
            output.last = record.Product;
        }
        if (eof) {
            std::cout << std::endl << "Can't handle dummy series" << std::endl;
            exit(1);
        }
    }
}

static bool mergeFiles(std::string& tapeIn1, std::string& tapeIn2, std::string& tapeOut, int& seriesIn1, int& seriesIn2, int& seriesOut, int& reads, int& writes, int& phases, bool display) {
    if (seriesIn1 == 0) {
        std::string tmpTape;
        tmpTape = tapeIn2;
        tapeIn2 = tapeOut;
        tapeOut = tmpTape;
        int tmpSeries;
        tmpSeries = seriesIn2;
        seriesIn2 = seriesOut;
        seriesOut = tmpSeries;

        return true;
    }
    if (seriesIn2 == 0) {
        std::string tmpTape;
        tmpTape = tapeIn1;
        tapeIn1 = tapeOut;
        tapeOut = tmpTape;
        int tmpSeries;
        tmpSeries = seriesIn1;
        seriesIn1 = seriesOut;
        seriesOut = tmpSeries;

        return true;
    }

    std::ifstream inFile1(tapeIn1, std::ios::binary);
    Block input1;
    input1.inFile = &inFile1;
    input1.last = -INFINITY;

    std::ifstream inFile2(tapeIn2, std::ios::binary);
    Block input2;
    input2.inFile = &inFile2;
    input2.last = -INFINITY;

    std::ofstream outFile(tapeOut, std::ios::binary);
    Block output;
    output.outFile = &outFile;
    output.last = -INFINITY;

    int dummies1 = 0;
    int dummies2 = 0;
    calculateDummySeries(seriesIn1, seriesIn2, dummies1, dummies2);
    handleDummySeries(input1, input2, output, seriesIn1, seriesIn2, seriesOut, dummies1, dummies2, reads, writes);

    int series = std::min(seriesIn1, seriesIn2);

    bool eof1 = false;
    bool eof2 = false;
    bool stop1 = false;
    bool stop2 = false;
    while (true) {
        eof1 = input1.readFromFile(*input1.inFile, reads);
        eof2 = input2.readFromFile(*input2.inFile, reads);
        while (!input1.isEmpty() && !input2.isEmpty()) {
            Record record1 = input1.first();
            if (record1.Product < input1.last) {
                // End of series
                stop1 = true;
                break;
            }
            Record record2 = input2.first();
            if (record2.Product < input2.last) {
                // End of series
                stop2 = true;
                break;
            }
            if (record1.Product <= record2.Product) {
                if (record1.Product < output.last) {
                    seriesOut++;
                }
                if (output.isFull()) {
                    output.writeToFile(*output.outFile, writes);
                }
                output.records.push_back(input1.pop());
                output.last = record1.Product;
                input1.last = record1.Product;
            }
            else if (record2.Product < record1.Product) {
                if (record2.Product < output.last) {
                    seriesOut++;
                }
                if (output.isFull()) {
                    output.writeToFile(*output.outFile, writes);
                }
                output.records.push_back(input2.pop());
                output.last = record2.Product;
                input2.last = record2.Product;
            }
        }
        if (eof1 && input1.isEmpty()) {
            stop1 = true;
        }
        if (eof2 && input2.isEmpty()) {
            stop2 = true;
        }
        if (!stop2 && stop1) {
            eof2 = input2.readFromFile(*input2.inFile, reads);
            while (!input2.isEmpty()) {
                Record record = input2.first();
                if (record.Product < input2.last) {
                    // End of series
                    stop2 = true;
                    break;
                }
                if (record.Product < output.last) {
                    seriesOut++;
                }
                if (output.isFull()) {
                    output.writeToFile(*output.outFile, writes);
                }
                output.records.push_back(input2.pop());
                output.last = record.Product;
                input2.last = record.Product;
            }
            if (eof2) {
                stop2 = true;
                break;
            }
        }
        if (!stop1 && stop2) {
            eof1 = input1.readFromFile(*input1.inFile, reads);
            while (!input1.isEmpty()) {
                Record record = input1.first();
                if (record.Product < input1.last) {
                    // End of series
                    stop1 = true;
                    break;
                }
                if (record.Product < output.last) {
                    seriesOut++;
                }
                if (output.isFull()) {
                    output.writeToFile(*output.outFile, writes);
                }
                output.records.push_back(input1.pop());
                output.last = record.Product;
                input1.last = record.Product;
            }
            if (eof1) {
                stop1 = true;
                break;
            }
        }
        if (stop1 && stop2) {
            series--;
            seriesIn1 -= 1;
            seriesIn2 -= 1;
            stop1 = false;
            stop2 = false;
            input1.last = -INFINITY;
            input2.last = -INFINITY;
        }
        if (series == 0) {
            if (!output.isEmpty()) {
                output.writeToFile(*output.outFile, writes);
            }
            if (seriesIn1 == 0 && seriesIn2 == 0) {
                inFile1.close();
                inFile2.close();
                std::remove(tapeIn1.c_str());
                std::remove(tapeIn2.c_str());
                std::ofstream tmpFile1(tapeIn1, std::ios::binary);
                std::ofstream tmpFile2(tapeIn2, std::ios::binary);
                tmpFile1.close();
                tmpFile2.close();
                outFile.close();
                break;
            }
            if (seriesIn1 > 0) {
                std::ofstream tmpFile("tmp.bin", std::ios::binary);
                while (!input1.isEmpty()) {
                    Record record = input1.pop();
                    record.writeToFile(tmpFile);
                }
                while (true) {
                    Record record;
                    record.readFromFile(*input1.inFile);
                    if (input1.inFile->eof()) {
                        break;
                    }
                    record.writeToFile(tmpFile);
                }
                tmpFile.close();
                inFile1.close();
                std::remove(tapeIn1.c_str());
                /*int begone_C6031 =*/ std::rename("tmp.bin", tapeIn1.c_str());
                /*if (begone_C6031 != 0) {
                    std::cout << std::endl << "Error renaming file" << std::endl;
                    exit(1);
                }*/
                inFile2.close();
                std::remove(tapeIn2.c_str());
                std::ofstream tmpFile2(tapeIn2, std::ios::binary);
                tmpFile2.close();
                outFile.close();
                break;
            }
            if (seriesIn2 > 0) {
                std::ofstream tmpFile("tmp.bin", std::ios::binary);
                while (!input2.isEmpty()) {
                    Record record = input2.pop();
                    record.writeToFile(tmpFile);
                }
                while (true) {
                    Record record;
                    record.readFromFile(*input2.inFile);
                    if (input2.inFile->eof()) {
                        break;
                    }
                    record.writeToFile(tmpFile);
                }
                tmpFile.close();
                inFile2.close();
                std::remove(tapeIn2.c_str());
                /*int begone_C6031 =*/ std::rename("tmp.bin", tapeIn2.c_str());
                /*if (begone_C6031 != 0) {
                    std::cout << std::endl << "Error renaming file" << std::endl;
                    exit(1);
                }*/
                inFile1.close();
                std::remove(tapeIn1.c_str());
                std::ofstream tmpFile1(tapeIn1, std::ios::binary);
                tmpFile1.close();
                outFile.close();
                break;
            }
        }
    }

    phases++;

    if (seriesIn1 == 0 && seriesIn2 == 0) {
        return true;
    }

    if (display) {
        std::cout << std::endl << "------ After merging --------";
        printTapes();
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }

    // Tries to make tape2 > tape1
    if (seriesIn1 == 0) {
        std::string tmpTape;
        int tmpSeries;
        tmpTape = tapeIn1;
        tapeIn1 = tapeIn2;
        tapeIn2 = tapeOut;
        tapeOut = tmpTape;
        tmpSeries = seriesIn1;
        seriesIn1 = seriesIn2;
        seriesIn2 = seriesOut;
        seriesOut = tmpSeries;

        return false;
    }
    if (seriesIn2 == 0) {
        std::string tmpTape;
        int tmpSeries;
        tmpTape = tapeIn2;
        tapeIn2 = tapeOut;
        tapeOut = tmpTape;
        tmpSeries = seriesIn2;
        seriesIn2 = seriesOut;
        seriesOut = tmpSeries;

        return false;
    }
    std::cout << std::endl << "No empty tape after merging" << std::endl;
    exit(1);
}

static void polyphaseMergeSort(std::string tapeA, std::string tapeB, std::string tapeC, bool display, bool printInOut) {
    int reads = 0;
    int writes = 0;
    int phases = 0;
    int seriesA = 0;
    int seriesB = 0;
    int seriesC = 0;
    int seriesInput = 0;
    int recordsInput = 0;
    double time0 = 0.0;

    getInputInfo(tapeC, seriesInput, recordsInput);
    int avgPhases = int(std::round(1.45 * log2(seriesInput)));
    int avgReadsWrites = int(std::round(2.0 * recordsInput * (1.04 * log2(seriesInput) + 1) / BLOCK_SIZE));

    if (printInOut) {
        printTape(tapeC, "Input file");
    }

    if (!display) {
        time0 = double(clock());
    }

    splitInputFile(tapeA, tapeB, tapeC, seriesA, seriesB, reads, writes, display);

    std::string tapeIn1 = tapeA;
    std::string tapeIn2 = tapeB;
    std::string tapeOut = tapeC;
    int& seriesIn1 = seriesA;
    int& seriesIn2 = seriesB;
    int& seriesOut = seriesC;

    bool sorted = false;
    while (!sorted) {
        sorted = mergeFiles(tapeIn1, tapeIn2, tapeOut, seriesIn1, seriesIn2, seriesOut, reads, writes, phases, display);
    }

    if (seriesOut != 1) {
        std::cout << std::endl << "More than 1 series in output" << std::endl;
        exit(1);
    }

    if (!display) {
        time0 = std::round((clock() - time0) / CLOCKS_PER_SEC * 100) / 100.0;
    }

    std::cout << std::endl << "------ After sorting --------";
    if (printInOut) {
        printTape(tapeOut, "Sorted file");
    }
    std::cout << std::endl << "Phases: " << phases;
    std::cout << std::endl << "Reads: " << reads;
    std::cout << std::endl << "Writes: " << writes;
    std::cout << std::endl << "Disk operations: " << reads + writes;
    std::cout << std::endl;
    std::cout << std::endl << "Theoretical phases: " << avgPhases;
    std::cout << std::endl << "Theoretical disk operations: " << avgReadsWrites;
    std::cout << std::endl;
    if (!display) {
        std::cout << std::endl << "Time: " << time0 << "s" << std::endl;
    }
}

int main() {
    srand(unsigned int(time(NULL)));
    int choice;
    bool afterPhase;
    bool printInOut = false;

    std::cout << "Polyphase merge sort" << std::endl;
    std::cout << "1. Generate records" << std::endl;
    std::cout << "2. Input records" << std::endl;
    std::cout << "3. Read from file" << std::endl;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    std::cin.ignore();
    switch (choice)
    {
    case 1:
        generateRecords(TAPE_C);
        break;
    case 2:
        inputRecords(TAPE_C);
        break;
    case 3:
        std::cout << "Filename: " << TAPE_C << std::endl;
        std::cout << "OK? (0/1): ";
        std::cin >> choice;
        if (choice == 0) {
            std::cout << "Too bad" << std::endl;
            exit(1);
        }
        break;
    default:
        std::cout << "Invalid choice" << std::endl;
        exit(1);
        break;
    }
    std::cout << "Display after phases? (0/1): ";
    std::cin >> afterPhase;
    std::cin.ignore();
    polyphaseMergeSort(TAPE_A, TAPE_B, TAPE_C, afterPhase, printInOut);
    return 0;
}
