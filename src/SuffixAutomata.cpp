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
      startLink(inputSize * 2),
      endLink(inputSize * 2),
      length(inputSize * 2),
      linkOrHalfLen(inputSize * 2),
      startTrans(inputSize * 2)
{
    // CompactArray<ALPHABET_SIZE>::1initialize(inputSize, inputSize * 2);
    // states.reserve(inputSize * 2);
    linksToChildren.reserve(inputSize * 2);
    linksToChildrenNext.reserve(inputSize * 2);
    trans.reserve(inputSize * 2);
    transTo.reserve(inputSize * 2);
    transNext.reserve(inputSize * 2);

    // Add state for empty word
    // states.emplace_back(-1, 0);
    linkOrHalfLen[0] = -1;
    length[0] = 0;
    startLink[0] = 0;
    endLink[0] = 0;
    startTrans[0] = 0;
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);
    trans.push_back(-1);
    transNext.push_back(-1);
    transTo.push_back(-1);
    ++stateCount;

    for (int i = 0; i < inputSize; ++i)
    {
        addLetter(input[i]);
    }

    calculateFinalsCount();
}

void SuffixAutomata::addLetter(char c)
{
    c -= FIRST_LETTER;

    int r = stateCount;
    // states.emplace_back(0, states[last].length + 1);
    length[r] = length[last] + 1;
    startLink[r] = linksToChildren.size();
    endLink[r] = linksToChildren.size();
    startTrans[r] = trans.size();
    trans.push_back(-1);
    transTo.push_back(-1);
    transNext.push_back(-1);
    linksToChildren.push_back(-1);
    linksToChildrenNext.push_back(-1);
    ++stateCount;

    int p = last;
    while (p > -1 && getTrans(p, c) == -1)
    {
        setTrans(p, c, r);
        // states[p].states.set(c, r);
        p = linkOrHalfLen[p];
        ++transitionsCount;
    }

    if (p != -1)
    {
        // int q = states[p].states.get(c);
        int q = getTrans(p, c);

        // if (states[p].length + 1 == states[q].length)
        if (length[p] + 1 == length[q])
        {
            linkOrHalfLen[r] = q;
            // states[r].linkOrHalfLenLink = q;
            addLinkToChild(q, r);
        }
        else
        {
            // int qq = states.size();
            int qq = stateCount;
            ++stateCount;
            // states.emplace_back(states[q]);

            startLink[qq] = linksToChildren.size();
            endLink[qq] = linksToChildren.size();
            linksToChildren.push_back(r);
            linksToChildrenNext.push_back(-1);
            length[qq] = length[p] + 1;

            // states.back().length = states[p].length + 1;

            // int qLink = states[q].linkOrHalfLenLink;
            // int qLink =
            swapLinkToChild(linkOrHalfLen[q], q, qq);

            // states[r].linkOrHalfLenLink = qq;
            // states[q].linkOrHalfLenLink = qq;
            linkOrHalfLen[r] = qq;
            linkOrHalfLen[q] = qq;
            addLinkToChild(qq, q);

            // transitionsCount += states.back().states.getSize();

            // while (p > -1 && states[p].states.get(c) == q)
            while (p > -1 && getTrans(p, c) == q)
            {
                setTrans(p, c, qq);
                // states[p].states.set(c, qq);
                // p = states[p].linkOrHalfLenLink;
                p = linkOrHalfLen[p];
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
    return stateCount;
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
            // State &topState = states[top];
            int topLen = length[top];

            // int seenFound = seen[topState.length >> 1];
            int seenFound = seen[topLen >> 1];
            if (topLen % 2 == 0 && seenFound != -1)
            {
                linkOrHalfLen[top] = seenFound;
            }
            else
            {
                linkOrHalfLen[top] = -1;
            }

            seen[topLen] = top;
            dfs.push({1, topLen});
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

    std::fill(startLink.begin(), startLink.begin() + inputSize + 2, -1);
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
            // State &topState = states[top];

            int seenFound = startLink[length[top] >> 1];
            if (length[top] % 2 == 0 && seenFound != -1)
            {
                halfLenLink = linkOrHalfLen[top];
                if (halfLenLink != 0 && seenFound == halfLenLink)
                {
                    ++count;
                }
            }

            startLink[length[top]] = top;
            dfs.push({1, length[top]});
            // for (i = 0, j = 0; i < ALPHABET_SIZE && j < topState.states.getSize(); ++i)
            for (i = startTrans[top]; i != -1; i = transNext[i])
            {
                trans = transTo[i];
                // trans = topState.states.get(i);

                // if (trans == -1)
                // {
                //     continue;
                // }

                // ++j;

                if (length[trans] == length[top])
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
        c = linkOrHalfLen[c];
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

void SuffixAutomata::setTrans(int state, char letter, int to)
{
    int start = startTrans[state];
    if (trans[start] == -1)
    {
        trans[start] = letter;
        transTo[start] = to;
    }
    
    for (int c = start; c != -1; c = transNext[c])
    {
        if (trans[c] == letter)
        {
            transTo[c] = to;
            return;
        }
    }


}

int SuffixAutomata::getTrans(int state, char letter)
{
    for (int c = startLink[state]; c != -1; c = transNext[c])
    {
        if (trans[c] == letter)
        {
            return transTo[c];
        }
    }
    return -1;
}