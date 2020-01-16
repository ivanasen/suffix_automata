#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include "CompactArray.hpp"

const int ALPHABET_SIZE = 26;
const int FIRST_LETTER = 97;

struct State
{
    int link;
    int length;
    CompactArray<ALPHABET_SIZE> states;

    State(int link, int length)
        : link(link), length(length)
    {
    }
};

struct SuffixAutomata
{
    int transitionsCount = 0;
    int last = 0;

    std::vector<State> states;
    char *input;
    int inputSize;
    std::unordered_set<int> leaves;
    std::vector<int> halfSuffixLinks;
    std::vector<std::vector<int>> links;
    // std::unordered_map<int, int> halfSuffixLinks;

    SuffixAutomata(char *input, int size);

    void addLetter(char c, int primaryCount);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount();

private:
    int findHalfLengthLink(int u) const;

    int traverse(int startState, int length) const;

    void markHalfLenSuffixLinks();
};

#endif