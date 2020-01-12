#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#include "SuffixAutomata.h"

SuffixAutomata::SuffixAutomata(char *input, int size)
    : input(input), inputSize(size)
{
    states.reserve(SIZE * 2);
    CompactArray<26>::bigTransitionState.reserve(SIZE);
    CompactArray<26>::smallTransitionState.reserve(SIZE * 2);

    states.emplace_back(-1, 0, true);

    for (int i = 0; i < inputSize; ++i)
    {
        addLetter(input[i]);
    }
}

void SuffixAutomata::addLetter(char c)
{
    c -= 97;

    int r = states.size();
    ++primaryCount;
    states.emplace_back(0, states[last].length + 1, true);
    states.back().start = 0;

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
            states.back().primary = false;
            states.back().start = primaryCount - states[p].length - 2;

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

int SuffixAutomata::getSquaresCount() const
{
    int count = 1;
    int halfInputSize = inputSize >> 1;
    for (int i = 0; i < states.size(); ++i)
    {
        if (states[i].primary)
        {
            if (states[i].length % 2)
            {
                continue;
            }

            int h = findHalfLengthLink(i);
            if (h == -1)
                continue;

            if (states[h].primary)
            {
                // The state is a prefix
                ++count;
            }
        }
        else
        {
            if (states[i].length >= halfInputSize)
            {
                continue;
            }

            int traversed = traverse(i, states[i].length);
            if (traversed != -1 && states[traversed].length == (states[i].length << 1))
            {
                ++count;
            }
        }
    }

    return count;
}

int SuffixAutomata::traverse(int startState, int length) const
{
    int currentState = startState;
    int currentPos = states[startState].start;

    while (currentPos - states[startState].start < states[startState].length)
    {
        int next = states[currentState].states.get(input[currentPos] - 97);
        if (next == -1)
        {
            return -1;
        }

        currentState = next;
        ++currentPos;
    }

    return currentState;
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