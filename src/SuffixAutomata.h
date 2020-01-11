#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include <bitset>
#include "CompactArray.hpp"

struct State
{
    int link;
    int length;
    int start;
    bool primary = false;
    CompactArray<26> states;

    State(int link, int length, bool primary)
        : link(link), length(length), primary(primary)
    {
    }
};

struct SuffixAutomata
{
    static const unsigned int SIZE = 200000000;
    int transitionsCount = 0;
    int last = 0;

    std::vector<State> states;
    std::string input;
    int primaryCount = 1;

    SuffixAutomata();

    void addLetter(char c);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount() const;

private:
    int findHalfLengthLink(int u) const;

    int traverse(int startState, int length) const;
};

#endif