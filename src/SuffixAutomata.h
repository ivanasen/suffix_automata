#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include "CompactArray.hpp"

struct State
{
    int link;
    int length;
    CompactArray<26> states;

    State(int link, int length)
        : link(link), length(length)
    {
    }
};

struct SuffixAutomata
{
    static const unsigned int SIZE = 200000000;
    int transitionsCount = 0;
    int squaresCount = 0;
    int last = 0;

    std::vector<State> states;

    SuffixAutomata();

    void addLetter(char c);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount() const;
};

#endif