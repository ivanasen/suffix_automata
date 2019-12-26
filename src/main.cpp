#include <iostream>
#include <fstream>
#include "SuffixAutomaton.h"

int main(int argc, char *argv[]) {
    std::ifstream input = std::ifstream(argv[1]);
    SuffixAutomaton sa(input);
    printf("%d\n%d\n%d", sa.getStatesCount(), sa.getTransitionsCount(), sa.getLongestSquaresCount());
    return 0;
}
