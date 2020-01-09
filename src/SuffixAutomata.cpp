#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#include "SuffixAutomata.h"

SuffixAutomata::SuffixAutomata()
{
    states.reserve(SIZE);
    states.emplace_back(-1, 0); // add the initial node
}

void SuffixAutomata::addLetter(char c)
{
    c -= 97;

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
            // we don't have to split q, just set the correct suffix link
            states[r].link = q;
        else
        {
            // we have to split, add q'
            int qq = states.size();
            states.push_back(states[q]);
            states.back().length = states[p].length + 1;

            states[r].link = qq;
            states[q].link = qq;

            transitionsCount += states.back().states.size;

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
    return squaresCount;
}