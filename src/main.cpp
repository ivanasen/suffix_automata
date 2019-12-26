#include <iostream>
#include <fstream>
#include "SuffixAutomaton.h"

int main(int argc, char *argv[]) {
    std::ifstream input = std::ifstream(argv[1]);
    SuffixAutomaton sa(input);
    printf("%d\n%d\n%d\n", sa.stateSize, sa.transitionCount, sa.squareCount);
    return 0;
}
