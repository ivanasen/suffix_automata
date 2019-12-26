#ifndef SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
#define SUFFIXAUTOMATON_SUFFIXAUTOMATON_H

#include <string>
#include <vector>
#include <unordered_map>

class SuffixAutomaton {
private:
//    std::vector<std::vector<int>> edges;
//    std::vector<int> link;
//    std::vector<int> length;
    int **vertices;
    int *link;
    int *length;
    int last;

    int verticesSize = 0;
    int linkSize = 0;
    int lengthSize = 0;

    int edgesCount = 0;
    int squaresCount = 0;

public:
//    explicit SuffixAutomaton(const std::string &s);

    explicit SuffixAutomaton(std::istream &istream);

    [[nodiscard]] unsigned getStatesCount() const;

    [[nodiscard]] unsigned getTransitionsCount() const;

    [[nodiscard]] unsigned getLongestSquaresCount() const;
};


#endif //SUFFIXAUTOMATON_SUFFIXAUTOMATON_H
