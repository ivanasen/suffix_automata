#include <iostream>
#include "SuffixAutomaton.h"

int main() {
    std::string s;
    std::cin >> s;

    SuffixAutomaton sa(s);

    std::cout
            << sa.getStatesCount() << '\n'
            << sa.getTransitionsCount() << '\n'
            << sa.getLongestSquaresCount() << '\n';

    return 0;
}
