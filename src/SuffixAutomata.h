#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
// #include "CompactArray.hpp"

const int ALPHABET_SIZE = 26;
const int FIRST_LETTER = 97;

// struct State
// {
//     int linkOrHalfLenLink;
//     int length;
//     // CompactArray<ALPHABET_SIZE> states;

//     State(int link, int length)
//         : linkOrHalfLenLink(link), length(length)
//     {
//     }
// };

struct SuffixAutomata
{
    int finalsCount = 0;
    int transitionsCount = 0;
    int last = 0;
    int stateCount = 0;

    // std::vector<State> states;
    char *input;
    int inputSize;
    std::vector<int> trans;
    std::vector<int> transTo;
    std::vector<int> transNext;
    std::vector<int> linksToChildren;
    std::vector<int> linksToChildrenNext;
    std::vector<int> startLink;
    std::vector<int> endLink;
    std::vector<int> length;
    std::vector<int> linkOrHalfLen;
    std::vector<int> startTrans;

    SuffixAutomata(char *input, int size);

    void addLetter(char c);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount();

private:
    int traverse(int startState, int length) const;

    void markHalfLenSuffixLinks();

    void calculateFinalsCount();

    void addLinkToChild(int state, int child);

    void swapLinkToChild(int state, int child, int newChild);

    void setTrans(int state, char trans, int to);

    int getTrans(int state, char trans);
};

#endif