#include <iostream>
#include <fstream>
#include "SuffixAutomaton.h"

//char getRandomChar() {
//    return 97 + rand() % 26;
//}
//
//void generateRandomString(std::string &s, int size) {
//    for (int i = 0; i < size; ++i) {
//        s.push_back(getRandomChar());
//    }
//}

int main(int argc, char *argv[]) {
//
//    int size = std::stoi(argv[1]);
//    std::string generated;
//    generated.reserve(size);
//
//    generateRandomString(generated, size);
//
//    puts("String generated!");
    std::ifstream input = std::ifstream(argv[1]);
    SuffixAutomaton sa(input);
    printf("%d\n%d\n%d", sa.getStatesCount(), sa.getTransitionsCount(), sa.getLongestSquaresCount());

    return 0;
}
