#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stack>
#include <algorithm>

#include "SuffixAutomata.h"

SuffixAutomata::SuffixAutomata(char *input, int size)
    : input(input), inputSize(size), startLink(size * 2 + 1), endLink(size * 2 + 1)
{
    CompactArray<ALPHABET_SIZE>::initialize(inputSize / 2, inputSize * 2);
    states.reserve(inputSize * 2);
    linksToChildren.reserve(inputSize * 2);
    linksToChildrenNext.reserve(inputSize * 2);

    // Add state for empty word
    states.emplace_back(-1, 0);
    endLink[0] = 0;
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);

    for (int i = 0; i < inputSize; ++i)
    {
        addLetter(input[i]);
    }

    calculateFinalsCount();
}

void SuffixAutomata::addLetter(char c)
{
    c -= FIRST_LETTER;

    int r = states.size();
    states.emplace_back(0, states[last].length + 1);
    startLink[r] = linksToChildren.size();
    endLink[r] = linksToChildren.size();
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);

    int p = last;
    while (p > -1 && states[p].states.get(c) == -1)
    {
        states[p].states.set(c, r);
        p = states[p].linkOrHalfLenLink;
        ++transitionsCount;
    }

    if (p != -1)
    {
        int q = states[p].states.get(c);

        if (states[p].length + 1 == states[q].length)
        {
            states[r].linkOrHalfLenLink = q;
            addLinkToChild(q, r);
        }
        else
        {
            int qq = states.size();
            states.emplace_back(states[q]);
            startLink[qq] = linksToChildren.size();
            endLink[qq] = linksToChildren.size();
            linksToChildren.push_back(r);
            linksToChildrenNext.push_back(-1);

            states.back().length = states[p].length + 1;

            int qLink = states[q].linkOrHalfLenLink;
            swapLinkToChild(qLink, q, qq);

            states[r].linkOrHalfLenLink = qq;
            states[q].linkOrHalfLenLink = qq;
            addLinkToChild(qq, q);

            transitionsCount += states.back().states.getSize();

            while (p > -1 && states[p].states.get(c) == q)
            {
                states[p].states.set(c, qq);
                p = states[p].linkOrHalfLenLink;
            }
        }
    }
    else
    {
        addLinkToChild(0, r);
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
    return finalsCount;
}

void SuffixAutomata::markHalfLenSuffixLinks()
{
    std::vector<int> seen(inputSize + 1, -1);
    std::stack<std::pair<int, int>> dfs;
    dfs.push({0, 0});

    std::pair<int, int> topP;
    int top;
    int i;
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
                topState.linkOrHalfLenLink = seenFound;
            }
            else
            {
                topState.linkOrHalfLenLink = -1;
            }

            seen[topState.length] = top;
            dfs.push({1, topState.length});
            if (linksToChildren[startLink[top]] != -1)
            {
                for (i = startLink[top]; i != -1; i = linksToChildrenNext[i])
                {
                    dfs.push({0, linksToChildren[i]});
                }
            }
        }
        else
        {
            seen[top] = -1;
        }
    }

    linksToChildren.clear();
    linksToChildrenNext.clear();
    endLink.clear();
}

int SuffixAutomata::getSquaresCount()
{
    // puts("Getting squares\n");
    markHalfLenSuffixLinks();
    // puts("Marked Half Length squares\n");

    int count = 1;

    startLink.resize(inputSize + 1);
    std::fill(startLink.begin(), startLink.end(), -1);
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

            int seenFound = startLink[topState.length >> 1];
            if (topState.length % 2 == 0 && seenFound != -1)
            {
                halfLenLink = states[top].linkOrHalfLenLink;
                if (halfLenLink != 0 && seenFound == halfLenLink)
                {
                    ++count;
                }
            }

            startLink[topState.length] = top;
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
            startLink[top] = -1;
        }
    }

    return count;
}

void SuffixAutomata::calculateFinalsCount()
{
    int c = last;
    while (c > -1)
    {
        ++finalsCount;
        c = states[c].linkOrHalfLenLink;
    }
}

void SuffixAutomata::addLinkToChild(int state, int child)
{
    int start = startLink[state];
    if (linksToChildren[start] == -1)
    {
        linksToChildren[start] = child;
        return;
    }

    int &end = endLink[state];
    linksToChildrenNext[end] = linksToChildren.size();
    end = linksToChildren.size();
    linksToChildren.push_back(child);
    linksToChildrenNext.push_back(-1);
}

void SuffixAutomata::swapLinkToChild(int state, int child, int newChild)
{
    for (int c = startLink[state]; c != -1; c = linksToChildrenNext[c])
    {
        if (linksToChildren[c] == child)
        {
            linksToChildren[c] = newChild;
            return;
        }
    }
}