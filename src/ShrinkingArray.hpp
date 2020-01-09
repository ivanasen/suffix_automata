#ifndef SHRINKING_ARRAY_H
#define SHRINKING_ARRAY_H

#include <cstring>
#include <stdio.h>

template <const int SIZE>
struct ShrinkingArray
{
    static const int DEFAULT = -1;

    int *arr;
    int idx;
    int size;

    ShrinkingArray() : arr(new int[1]), idx(DEFAULT), size(0)
    {
        arr[0] = DEFAULT;
    }

    ShrinkingArray(const ShrinkingArray &other) : idx(other.idx), size(other.size)
    {
        if (size <= 1)
        {
            arr = new int[1];
            arr[0] = other.arr[0];
        }
        else
        {
            arr = new int[SIZE];
            for (int i = 0; i < SIZE; ++i)
                arr[i] = other.arr[i];
        }
    }

    int get(int index)
    {
        if (size == 1 && idx == index)
            return arr[0];
        if (size > 1)
            return arr[index];

        return DEFAULT;
    }

    void set(int index, int value)
    {
        if (size == 0)
        {
            idx = index;
            size = 1;
            arr = new int[1];
            arr[0] = value;
            return;
        }

        if (size == 1 && index == idx)
        {
            arr[0] = value;
            return;
        }

        if (size == 1)
        {
            int *newArr = new int[SIZE];
            memset(newArr, DEFAULT, SIZE * sizeof(int));

            newArr[idx] = arr[0];

            delete[] arr;

            arr = newArr;
            idx = DEFAULT;
        }

        if (arr[index] == DEFAULT)
            ++size;
        arr[index] = value;
    }
};

#endif