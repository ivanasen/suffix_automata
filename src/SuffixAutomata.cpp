#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stack>

#include "SuffixAutomata.h"

SuffixAutomata::SuffixAutomata(char *input, int size)
    : input(input), inputSize(size)
{
    CompactArray<ALPHABET_SIZE>::initialize(SIZE, SIZE * 2);
    states.reserve(SIZE * 2);

    // Add state for empty word
    states.emplace_back(-1, 0);

    for (int i = 0; i < inputSize; ++i)
    {
        // primary count is i + 2, because we're counting the state for
        // the empty word too
        addLetter(input[i], i + 2);
    }
}

void SuffixAutomata::addLetter(char c, int primaryCount)
{
    c -= FIRST_LETTER;

    int r = states.size();
    states.emplace_back(0, states[last].length + 1);

    // add edges to r and find p with link to q
    int p = last;
    while (p > -1 && states[p].states.get(c) == -1)
    {
        states[p].states.set(c, r);
        p = states[p].link;
        ++transitionsCount;
    }

    if (p != -1)
    {
        int q = states[p].states.get(c);

        if (states[p].length + 1 == states[q].length)
        {
            // we don't have to split q, just set the correct suffix link
            states[r].link = q;
        }
        else
        {
            // we have to split, add q'
            int qq = states.size();
            states.emplace_back(states[q]);
            states.back().length = states[p].length + 1;

            states[r].link = qq;
            states[q].link = qq;

            transitionsCount += states.back().states.getSize();

            // move short classes pointing to q to point to q'
            while (p > -1 && states[p].states.get(c) == q)
            {
                states[p].states.set(c, qq);
                p = states[p].link;
            }
        }
    }

    last = r;
}

int SuffixAutomata::getStatesCount() const
{
    return states.size();
}

int SuffixAutomata::getTransitionsCount() const
{
    return transitionsCount;
}

int SuffixAutomata::getFinalsCount() const
{
    int count = 0;
    int c = last;
    while (c > -1)
    {
        ++count;
        c = states[c].link;
    }
    return count;
}

void SuffixAutomata::markHalfLengthStates(int start, int &count, std::unordered_map<int, int> &seen)
{
    State s = states[start];
    if (s.length % 2 == 0 && seen.find(s.length >> 1) != seen.end())
    {
        int halfLenLink = findHalfLengthLink(start);
        if (seen[s.length >> 1] == halfLenLink)
        {
            ++count;
        }
    }

    seen[s.length] = start;
    for (int i = 0, j = 0; i < ALPHABET_SIZE && j < s.states.getSize(); ++i)
    {
        int trans = s.states.get(i);

        if (trans == -1)
        {
            continue;
        }

        ++j;

        if (states[trans].length == s.length + 1)
        {
            markHalfLengthStates(trans, count, seen);
        }
    }
    seen.erase(s.length);
}

int SuffixAutomata::getSquaresCount()
{
    int count = 1;

    std::unordered_map<int, int> seen;
    markHalfLengthStates(0, count, seen);

    return count;
}

int SuffixAutomata::findHalfLengthLink(int u) const
{
    int half = states[u].length >> 1;
    int v = states[u].link;
    while (v != -1 && states[v].length > half)
    {
        v = states[v].link;
    }

    if (v != -1 && states[v].length == half)
        return v;
    return -1;
}