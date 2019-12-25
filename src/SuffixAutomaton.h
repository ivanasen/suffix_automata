#ifndef SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
#define SUFFIXAUTOMATON_SUFFIXAUTOMATON_H

#include <string>

class SuffixAutomaton {
private:
    unsigned statesCount = 0;
    unsigned transitionsCount = 0;
    unsigned longestSquaresCount = 0;

public:
    explicit SuffixAutomaton(const std::string &s);

    [[nodiscard]] unsigned getStatesCount() const;

    [[nodiscard]] unsigned getTransitionsCount() const;

    [[nodiscard]] unsigned getLongestSquaresCount() const;
};


#endif //SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
