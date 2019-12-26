#include <iostream>
#include <fstream>
#include "SuffixAutomaton.h"

char getRandomChar() {
    return 97 + rand() % 26;
}

void generateRandomString(std::ostream &os, int size) {
    for (int i = 0; i < size; ++i) {
        os << getRandomChar();
    }
}

int main() {
    std::string fileName;
    int size;

    std::cin >> fileName >> size;
    std::ofstream output(fileName);

    generateRandomString(output, size);
    puts("Done.");

    return 0;
}
