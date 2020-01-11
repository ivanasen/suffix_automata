#ifndef COMPACT_ARRAY_H
#define COMPACT_ARRAY_H

#include <cstring>
#include <vector>
#include <stdio.h>

template <const int SIZE>
struct CompactArray
{
    inline static std::vector<int> globalTransitions;

    static const int SHRINKING_SIZE = 3;

    int id0OrStartIndex = -1;
    int id1OrIsBigger = -1;
    int id2OrSize = -1;

    int *arr;

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
            arr = new int[SHRINKING_SIZE];
            arr[0] = other.arr[0];
            arr[1] = other.arr[1];
            arr[2] = other.arr[2];
            return;
        }

        id0OrStartIndex = globalTransitions.size();
        for (int i = 0; i < SIZE; ++i)
            globalTransitions.push_back(globalTransitions[other.id0OrStartIndex + i]);
    }

    int get(int index)
    {
        int size = getSize();
        if (size > 0)
        {
            if (size <= SHRINKING_SIZE)
            {
                if (index == id0OrStartIndex)
                    return arr[0];
                if (index == id1OrIsBigger)
                    return arr[1];
                if (index == id2OrSize)
                    return arr[2];
                return -1;
            }
            return globalTransitions[id0OrStartIndex + index];
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
                arr[0] = value;
                return;
            }
            if (index == id1OrIsBigger)
            {
                arr[1] = value;
                return;
            }
            if (index == id2OrSize)
            {
                arr[2] = value;
                return;
            }

            if (size < SHRINKING_SIZE)
            {
                if (size == 0)
                {
                    arr = new int[SHRINKING_SIZE];
                    id0OrStartIndex = index;
                    arr[0] = value;
                    arr[1] = -1;
                    arr[2] = -1;
                }
                else if (size == 1)
                {
                    id1OrIsBigger = index;
                    arr[1] = value;
                }
                else
                {
                    id2OrSize = index;
                    arr[2] = value;
                }

                return;
            }

            int oldId0 = id0OrStartIndex;            
            id0OrStartIndex = globalTransitions.size();
            for (char i = 0; i < SIZE; ++i)
                globalTransitions.push_back(-1);

            globalTransitions[id0OrStartIndex + oldId0] = arr[0];
            globalTransitions[id0OrStartIndex + id1OrIsBigger] = arr[1];
            globalTransitions[id0OrStartIndex + id2OrSize] = arr[2];

            id1OrIsBigger = -1;
            id2OrSize = SHRINKING_SIZE;

            delete[] arr;
        }

        if (globalTransitions[id0OrStartIndex + index] == -1)
            ++id2OrSize;
        globalTransitions[id0OrStartIndex + index] = value;
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