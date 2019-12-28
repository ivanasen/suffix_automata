#include <cstring>
#include <iostream>
#include <stdio.h>

using namespace std;

template <const int SIZE>
struct shrinking_array
{
    static const int DEFAULT = -1;

    int *arr;
    int idx;
    int size;

    shrinking_array() : arr(new int[1]), idx(DEFAULT), size(0)
    {
        arr[0] = DEFAULT;
    }

    shrinking_array(const shrinking_array &other)
    {
        if (other.size <= 1)
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

        idx = other.idx;
        size = other.size;
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
            int *new_arr = new int[SIZE];
            memset(new_arr, DEFAULT, SIZE * sizeof(int));

            new_arr[idx] = arr[0];

            delete[] arr;

            arr = new_arr;
            idx = DEFAULT;
        }

        if (arr[index] == DEFAULT)
            ++size;
        arr[index] = value;
    }
};