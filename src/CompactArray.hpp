#ifndef COMPACT_ARRAY_H
#define COMPACT_ARRAY_H

#include <cstring>
#include <vector>
#include <stdio.h>

template <const int SIZE>
struct CompactArray
{
    inline static std::vector<int> bigTransitionState;
    inline static std::vector<int> smallTransitionState;

    static const int SHRINKING_SIZE = 3;

    int id0OrStartIndex = -1;
    int id1OrIsBigger = -1;
    int id2OrSize = -1;

    int smallArrIndex = -1;

    static void initialize(int bigTransitionsCapacity, int smallTransitionsCapacity)
    {
        bigTransitionState.reserve(bigTransitionsCapacity);
        smallTransitionState.reserve(smallTransitionsCapacity);
    }

    CompactArray() = default;

    CompactArray(const CompactArray &other)
        : id0OrStartIndex(other.id0OrStartIndex),
          id1OrIsBigger(other.id1OrIsBigger),
          id2OrSize(other.id2OrSize)
    {
        int size = getSize();

        if (size == 0)
            return;

        if (size <= SHRINKING_SIZE)
        {
            smallArrIndex = smallTransitionState.size();
            smallTransitionState.push_back(smallTransitionState[other.smallArrIndex]);
            smallTransitionState.push_back(smallTransitionState[other.smallArrIndex + 1]);
            smallTransitionState.push_back(smallTransitionState[other.smallArrIndex + 2]);
            return;
        }

        id0OrStartIndex = bigTransitionState.size();
        for (int i = 0; i < SIZE; ++i)
            bigTransitionState.push_back(bigTransitionState[other.id0OrStartIndex + i]);
    }

    int get(int index) const
    {
        int size = getSize();
        if (size > 0)
        {
            if (size <= SHRINKING_SIZE)
            {
                if (index == id0OrStartIndex)
                    return smallTransitionState[smallArrIndex];
                if (index == id1OrIsBigger)
                    return smallTransitionState[smallArrIndex + 1];
                if (index == id2OrSize)
                    return smallTransitionState[smallArrIndex + 2];
                return -1;
            }
            return bigTransitionState[id0OrStartIndex + index];
        }

        return -1;
    }

    void set(int index, int value)
    {
        int size = getSize();
        if (size <= SHRINKING_SIZE)
        {
            if (index == id0OrStartIndex)
            {
                smallTransitionState[smallArrIndex] = value;
                return;
            }
            if (index == id1OrIsBigger)
            {
                smallTransitionState[smallArrIndex + 1] = value;
                return;
            }
            if (index == id2OrSize)
            {
                smallTransitionState[smallArrIndex + 2] = value;
                return;
            }

            if (size < SHRINKING_SIZE)
            {
                if (size == 0)
                {
                    smallArrIndex = smallTransitionState.size();
                    id0OrStartIndex = index;
                    smallTransitionState.push_back(value);
                    smallTransitionState.push_back(-1);
                    smallTransitionState.push_back(-1);
                }
                else if (size == 1)
                {
                    id1OrIsBigger = index;
                    smallTransitionState[smallArrIndex + 1] = value;
                }
                else
                {
                    id2OrSize = index;
                    smallTransitionState[smallArrIndex + 2] = value;
                }

                return;
            }

            int oldId0 = id0OrStartIndex;
            id0OrStartIndex = bigTransitionState.size();
            for (char i = 0; i < SIZE; ++i)
                bigTransitionState.push_back(-1);

            bigTransitionState[id0OrStartIndex + oldId0] = smallTransitionState[smallArrIndex];
            bigTransitionState[id0OrStartIndex + id1OrIsBigger] = smallTransitionState[smallArrIndex + 1];
            bigTransitionState[id0OrStartIndex + id2OrSize] = smallTransitionState[smallArrIndex + 2];

            id1OrIsBigger = -1;
            id2OrSize = SHRINKING_SIZE;
        }

        if (bigTransitionState[id0OrStartIndex + index] == -1)
            ++id2OrSize;
        bigTransitionState[id0OrStartIndex + index] = value;
    }

    int getSize() const
    {
        if (id0OrStartIndex == -1)
            return 0;
        if (id1OrIsBigger == -1 && id2OrSize == -1)
            return 1;
        if (id2OrSize == -1)
            return 2;

        if (id1OrIsBigger != -1)
            return 3;

        return id2OrSize;
    }
};

#endif