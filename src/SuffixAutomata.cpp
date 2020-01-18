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
    : input(input),
      inputSize(size),
      startLink(size * 2 + 1),
      endLink(size * 2 + 1)
{
    states.reserve(inputSize * 2);
    linksToChildren.reserve(inputSize * 2);
    linksToChildrenNext.reserve(inputSize * 2);
    transChar.reserve(inputSize * 3);
    transVal.reserve(inputSize * 3);
    transNext.reserve(inputSize * 3);

    newState(0);

    for (int i = 0; i < inputSize; ++i)
    {
        addLetter(input[i]);
    }

    calculateFinalsCount();
}

void SuffixAutomata::addLetter(char c)
{
    c -= FIRST_LETTER;

    int r = newState(states[last].length + 1);

    int p = last;
    while (p > -1 && setTransIfNotExists(p, c, r))
    {
        p = states[p].linkOrHalfLenLink;
        ++transitionsCount;
    }

    if (p != -1)
    {
        int q = getTrans(p, c);

        if (states[p].length + 1 == states[q].length)
        {
            states[r].linkOrHalfLenLink = q;
            addLinkToChild(q, r);
        }
        else
        {
            int qq = cloneState(q);
            states[qq].length = states[p].length + 1;

            swapLinkToChild(states[q].linkOrHalfLenLink, q, qq);

            states[r].linkOrHalfLenLink = qq;
            states[q].linkOrHalfLenLink = qq;
            addLinkToChild(qq, q);
            addLinkToChild(qq, r);

            while (p > -1 && setTransIfEq(p, c, q, qq))
            {
                p = states[p].linkOrHalfLenLink;
            }
        }
    }
    else
    {
        states[r].linkOrHalfLenLink = 0;
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
    transChar.clear();

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
    markHalfLenSuffixLinks();

    int count = 1;

    startLink.resize(inputSize + 1);
    std::fill(startLink.begin(), startLink.end(), -1);
    std::stack<std::pair<int, int>> dfs;
    dfs.push({0, 0});

    std::pair<int, int> topP;
    int top;
    int halfLenLink;
    int trans;
    int i;
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
            for (i = topState.transStart; i != -1; i = transNext[i])
            {
                trans = transVal[i];

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

int SuffixAutomata::getTrans(int state, char c)
{
    for (int i = states[state].transStart; i != -1; i = transNext[i])
    {
        if (transChar[i] == c)
        {
            return transVal[i];
        }
    }
    return -1;
}

void SuffixAutomata::setTrans(int state, char c, int val)
{
    int i = states[state].transStart;
    if (transChar[i] == -1)
    {
        transChar[i] = c;
        transVal[i] = val;
        return;
    }

    for (; transNext[i] != -1; i = transNext[i])
    {
        if (transChar[i] == c)
        {
            transVal[i] = val;
            return;
        }
    }

    if (transChar[i] == c)
    {
        transVal[i] = val;
        return;
    }

    transNext[i] = transChar.size();
    transChar.push_back(c);
    transVal.push_back(val);
    transNext.push_back(-1);
}

bool SuffixAutomata::setTransIfEq(int state, char c, int val, int newVal)
{
    int i = states[state].transStart;
    if (transChar[i] == -1)
    {
        return false;
    }

    for (; i != -1; i = transNext[i])
    {
        if (transChar[i] == c)
        {
            if (transVal[i] == val)
            {
                transVal[i] = newVal;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool SuffixAutomata::setTransIfNotExists(int state, char c, int val)
{
    int i = states[state].transStart;
    if (transChar[i] == -1)
    {
        transChar[i] = c;
        transVal[i] = val;
        return true;
    }

    for (; transNext[i] != -1; i = transNext[i])
    {
        if (transChar[i] == c)
        {
            return false;
        }
    }

    if (transChar[i] == c)
    {
        return false;
    }

    transNext[i] = transChar.size();
    transChar.push_back(c);
    transVal.push_back(val);
    transNext.push_back(-1);
    return true;
}

int SuffixAutomata::newState(int length)
{
    int r = states.size();
    states.emplace_back(-1, length, transChar.size());
    startLink[r] = linksToChildren.size();
    endLink[r] = linksToChildren.size();
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);

    transChar.push_back(-1);
    transVal.push_back(-1);
    transNext.push_back(-1);

    return r;
}

int SuffixAutomata::cloneState(int state)
{
    int clone = states.size();
    states.emplace_back(states[state]);

    startLink[clone] = linksToChildren.size();
    endLink[clone] = linksToChildren.size();
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);

    states[clone].transStart = transChar.size();
    transChar.push_back(-1);
    transVal.push_back(-1);
    transNext.push_back(-1);

    for (int i = states[state].transStart; i != -1; i = transNext[i])
    {
        setTrans(clone, transChar[i], transVal[i]);
        ++transitionsCount;
    }

    return clone;
}