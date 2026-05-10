#include "../include/textgen.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    const int NPREF = 2;
    const int MAXGEN = 1200;

    std::string inputFile;
    std::string outputFile = "result/gen.txt";

    if (argc >= 2) {
        inputFile = argv[1];
    }
    else {
        std::cout << "Enter input file name: ";
        std::cin >> inputFile;
    }

    if (argc >= 3) {
        outputFile = argv[2];
    }

    try {
        TextGenerator generator(NPREF, MAXGEN);

        std::cout << "Loading text from " << inputFile << "..." << std::endl;
        generator.loadText(inputFile);

        std::cout << "Generating text..." << std::endl;
        std::string generatedText = generator.generate();

        std::ofstream out(outputFile);
        if (!out.is_open()) {
            throw std::runtime_error("Cannot create output file: " + outputFile);
        }

        out << generatedText;
        out.close();

        std::cout << "Generated text saved to " << outputFile << std::endl;
        std::cout << "\n--- Generated Text Preview ---\n";
        std::cout << generatedText.substr(0, 500) << "...\n";
        std::cout << "----------------------------\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}