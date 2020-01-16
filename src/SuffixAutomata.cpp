#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stack>

#include "SuffixAutomata.h"

SuffixAutomata::SuffixAutomata(char *input, int size)
    : input(input), inputSize(size), halfSuffixLinks(size * 2)
{
    CompactArray<ALPHABET_SIZE>::initialize(inputSize, inputSize * 2);
    states.reserve(inputSize * 2);
    leaves.reserve(inputSize);

    // Add state for empty word
    states.emplace_back(-1, 0);
    leaves.insert(0);

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
    leaves.insert(r);

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

            auto iter = leaves.find(q);
            if (iter != leaves.end())
            {
                leaves.erase(iter);
            }
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

void SuffixAutomata::markHalfLenSuffixLinks()
{
    int curr;
    int currLen;
    std::unordered_map<int, int> lenToState;

    for (int leaf : leaves)
    {
        curr = leaf;
        lenToState.clear();

        while (curr != -1)
        {
            currLen = states[curr].length;
            auto iter = lenToState.find(currLen * 2);
            if (iter != lenToState.end())
            {
                halfSuffixLinks[iter->second] = curr;
            }

            if (!(currLen % 2))
            {
                lenToState[currLen] = curr;
            }

            curr = states[curr].link;
        }
    }
}

int SuffixAutomata::getSquaresCount()
{
    puts("Getting squares\n");
    markHalfLenSuffixLinks();
    puts("Marked Half Length squares\n");

    int count = 1;

    std::vector<int> seen(inputSize + 1, -1);
    std::stack<std::pair<int, int>> dfs;
    dfs.push({0, 0});

    std::pair<int, int> topP;
    int top;
    int halfLenLink;
    int trans;
    int i, j;
    while (!dfs.empty())
    {
        topP = dfs.top();
        top = topP.second;
        dfs.pop();

        if (topP.first == 0)
        {
            State &topState = states[top];

            int seenFound = seen[topState.length >> 1];
            if (topState.length % 2 == 0 && seenFound != -1)
            {
                halfLenLink = halfSuffixLinks[top];
                if (halfLenLink != 0 && seenFound == halfLenLink)
                {
                    ++count;
                }
            }

            seen[topState.length] = top;
            dfs.push({1, topState.length});
            for (i = 0, j = 0; i < ALPHABET_SIZE && j < topState.states.getSize(); ++i)
            {
                trans = topState.states.get(i);

                if (trans == -1)
                {
                    continue;
                }

                ++j;

                if (states[trans].length == topState.length + 1)
                {
                    dfs.push({0, trans});
                }
            }
        }
        else
        {
            seen[top] = -1;
        }
    }

    return count;
}

int SuffixAutomata::findHalfLengthLink(int u) const
{
    // int half = states[u].length >> 1;
    // int v = states[u].link;
    // while (v != -1 && states[v].length > half)
    // {
    //     v = states[v].link;
    // }

    // if (v != -1 && states[v].length == half)
    //     return v;
    // return -1;
    //------------------------------------
    // auto iter = halfSuffixLinks.find(u);
    // if (iter == halfSuffixLinks.end())
    // {
    //     return -1;
    // }
    // return iter->second;
    return halfSuffixLinks[u];
}