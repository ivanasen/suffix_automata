#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include <bitset>
#include "CompactArray.hpp"

const int ALPHABET_SIZE = 26;
const unsigned int SIZE = 200000000;
const int FIRST_LETTER = 97;

struct State
{
    int link;
    int length;
    int start;
    bool primary = false;
    CompactArray<ALPHABET_SIZE> states;

    State(int link, int length, bool primary)
        : link(link), length(length), primary(primary)
    {
    }
};

struct SuffixAutomata
{
    int transitionsCount = 0;
    int last = 0;

    std::vector<State> states;
    char* input;
    int inputSize;

    SuffixAutomata(char* input, int size);

    void addLetter(char c, int primaryCount);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount() const;

private:
    int findHalfLengthLink(int u) const;

    int traverse(int startState, int length) const;
};

#endif