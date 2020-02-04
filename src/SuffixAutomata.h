#ifndef SUFFIX_AUTOMATA_H
#define SUFFIX_AUTOMATA_H

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

const int ALPHABET_SIZE = 26;
const int FIRST_LETTER = 97;

struct State
{
    int linkOrHalfLenLink;
    int length;
    int transStart;

    State(int link, int length, int transStart)
        : linkOrHalfLenLink(link), length(length), transStart(transStart)
    {
    }
};

struct SuffixAutomata
{
    int finalsCount = 0;
    int transitionsCount = 0;
    int last = 0;

    std::vector<State> states;
    char *input;
    int inputSize;
    std::vector<int> linksToChildren;
    std::vector<int> linksToChildrenNext;
    std::vector<int> startLink;
    std::vector<int> endLink;

    std::vector<char> transChar;
    std::vector<int> transVal;
    std::vector<int> transNext;

    SuffixAutomata(char *input, int size);

    void addLetter(char c);

    int getStatesCount() const;

    int getTransitionsCount() const;

    int getFinalsCount() const;

    int getSquaresCount();

private:
    void markHalfLenSuffixLinks();

    void calculateFinalsCount();

    void addLinkToChild(int state, int child);

    void swapLinkToChild(int state, int child, int newChild);

    int getTrans(int state, char c);

    void setTrans(int state, char c, int val);

    int newState(int length);

    int cloneState(int state);

    bool setTransIfNotExists(int state, char c, int val);

    bool setTransIfEq(int state, char c, int val, int newVal);
};

#endif