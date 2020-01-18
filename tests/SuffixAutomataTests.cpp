#include <iostream>
#include "../src/SuffixAutomata.h"

using namespace std;

void testSingle(char *s, int states, int transitions, int finals)
{
    SuffixAutomata a(s, strlen(s));
    cout
        << (a.getStatesCount() == states)
        << (a.getTransitionsCount() == transitions)
        << (a.getFinalsCount() == finals);
}

void testStatesTransitionsAndFinals()
{
    testSingle("", 1, 0, 1);
    testSingle("b", 2, 1, 2);
    testSingle("aa", 3, 2, 3);
    testSingle("abcbc", 8, 9, 3);
    testSingle("bbacbba", 8, 10, 3);
    testSingle("bbacbbaa", 10, 14, 3);
    testSingle("cpalgorithmspageauthorssuffixautomatontableofcontentsdefinition", 88, 144, 4);
    testSingle("abcbcbcbcbcbcbcbc", 32, 33, 9);

    cout << '\n';
}

void squaresSingle(char *s, int expectedSquares)
{
    SuffixAutomata a(s, strlen(s));
    cout << (a.getSquaresCount() == expectedSquares);
}

void testSquares()
{
    squaresSingle("", 1);
    squaresSingle("abcbc", 1);
    squaresSingle("aaaa", 3);
    squaresSingle("cacaac", 2);
    squaresSingle("bbaacaa", 3);
    squaresSingle("bcbcc", 2);
    squaresSingle("bcbcbcbc", 3);
    squaresSingle("abcabc", 2);
    squaresSingle("abbcbb", 2);
    squaresSingle("abbb", 2);
    squaresSingle("abcbcbcbc", 2);
    squaresSingle("abcdbcdebcdbcd", 2);
    squaresSingle("abbbbb", 3);
    squaresSingle("ebcbcbabc", 1);
    squaresSingle("ebcbcbcbcebabcbabc", 2);
    squaresSingle("baacaaddeddeddedd", 4);
    squaresSingle("baaaab", 2);

    cout << '\n';
}

void runSuffixAutomataTests()
{
    testStatesTransitionsAndFinals();
    testSquares();
}