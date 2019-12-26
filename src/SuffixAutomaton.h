#ifndef SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
#define SUFFIXAUTOMATON_SUFFIXAUTOMATON_H

#include <string>
#include <vector>
#include <unordered_map>

struct SuffixAutomaton {
    int **states;
    int *link;
    int *length;
    int last;

    int stateSize = 0;
    int linkSize = 0;
    int lengthSize = 0;

    int transitionCount = 0;
    int squareCount = 0;

    explicit SuffixAutomaton(std::istream &istream);
};


#endif //SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
