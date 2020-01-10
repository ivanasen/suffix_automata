#ifndef SHRINKING_ARRAY_H
#define SHRINKING_ARRAY_H

#include <cstring>
#include <stdio.h>

template <const int SIZE>
struct CompactArray
{
    static const int DEFAULT = -1;
    static const int SHRINKING_SIZE = 3;

    int *arr;
    int id0 = DEFAULT;
    int id1 = DEFAULT;
    int id2 = DEFAULT;
    int size = 0;

    CompactArray() = default;

    CompactArray(const CompactArray &other)
        : id0(other.id0),
          id1(other.id1),
          id2(other.id2),
          size(other.size)
    {
        if (size == 0)
            return;

        if (size <= SHRINKING_SIZE)
        {
            arr = new int[SHRINKING_SIZE];
            arr[0] = other.arr[0];
            arr[1] = other.arr[1];
            arr[2] = other.arr[2];
        }

        arr = new int[SIZE];
        for (int i = 0; i < SIZE; ++i)
            arr[i] = other.arr[i];
    }

    int get(int index)
    {
        if (size > 0)
        {
            if (size <= SHRINKING_SIZE)
            {
                if (index == id0)
                    return arr[0];
                if (index == id1)
                    return arr[1];
                if (index == id2)
                    return arr[2];
                return DEFAULT;
            }
            return arr[index];
        }

        return DEFAULT;
    }

    void set(int index, int value)
    {
        if (size <= SHRINKING_SIZE)
        {
            if (index == id0)
            {
                arr[0] = value;
                return;
            }
            if (index == id1)
            {
                arr[1] = value;
                return;
            }
            if (index == id2)
            {
                arr[2] = value;
                return;
            }

            if (size < SHRINKING_SIZE)
            {
                if (size == 0)
                {
                    arr = new int[SHRINKING_SIZE];
                    id0 = index;
                    arr[0] = value;
                    arr[1] = DEFAULT;
                    arr[2] = DEFAULT;
                }
                else if (size == 1)
                {
                    id1 = index;
                    arr[1] = value;
                }
                else
                {
                    id2 = index;
                    arr[2] = value;
                }

                ++size;
                return;
            }

            int *newArr = new int[SIZE];
            memset(newArr, DEFAULT, SIZE * sizeof(int));

            newArr[id0] = arr[0];
            newArr[id1] = arr[1];
            newArr[id2] = arr[2];

            delete[] arr;

            arr = newArr;
        }

        if (arr[index] == DEFAULT)
            ++size;
        arr[index] = value;
    }
};

#endif