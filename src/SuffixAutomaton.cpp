#include <cstring>
#include <string>
#include <iostream>
#include "SuffixAutomaton.h"

const int SIZE = 400000000;

SuffixAutomaton::SuffixAutomaton(std::istream &istream) {
    states = new int *[SIZE];
    link = new int[SIZE];
    length = new int[SIZE];

    // add the initial node
    states[stateSize] = new int[26];
    std::memset(states[stateSize++], -1, 26 * sizeof(int));
    link[linkSize++] = -1;
    length[lengthSize++] = 0;
    last = 0;

    char curr;
    int r;
    int p;
    int q;
    int qq;

    int i = 0;

    while (istream.get(curr) && curr != '\n') {
        curr -= 97;

        // construct r
        states[stateSize] = new int[26];
        std::memset(states[stateSize++], -1, 26 * sizeof(int));
        length[lengthSize++] = i + 1;
        link[linkSize++] = 0;
        r = stateSize - 1;

        // add edges to r and find p with link to q
        p = last;
        while (p >= 0 && states[p][curr] == -1) {
            states[p][curr] = r;
            p = link[p];

            ++transitionCount;
        }

        if (p != -1) {
            q = states[p][curr];

            if (length[p] + 1 == length[q]) {
                // we don't have to split q, just set the correct suffix link
                link[r] = q;
            } else {
                // we have to split, add q'
                states[stateSize++] = states[q];
                length[lengthSize++] = length[p] + 1;
                link[linkSize++] = link[q]; // copy parent of q

                qq = stateSize - 1;
                link[q] = qq;
                link[r] = qq;

                ++transitionCount;

                // move short classes pointing to q to point to q'
                while (p >= 0 && states[p][curr] == q) {
                    states[p][curr] = qq;
                    p = link[p];
                }

            }
        }

        last = r;
        ++i;
    }
}