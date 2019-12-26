#include <cstring>
#include <string>
#include <iostream>
#include "SuffixAutomaton.h"

const int SIZE = 400000000;

SuffixAutomaton::SuffixAutomaton(std::istream &istream) {
    vertices = new int *[SIZE];
    link = new int[SIZE];
    length = new int[SIZE];

    // add the initial node
    vertices[verticesSize] = new int[26];
    std::memset(vertices[verticesSize++], -1, 26 * sizeof(int));
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
        vertices[verticesSize] = new int[26];
        std::memset(vertices[verticesSize++], -1, 26 * sizeof(int));
        length[lengthSize++] = i + 1;
        link[linkSize++] = 0;
        r = verticesSize - 1;

        // add edges to r and find p with link to q
        p = last;
        while (p >= 0 && vertices[p][curr] == -1) {
            vertices[p][curr] = r;
            p = link[p];

            ++edgesCount;
        }

        if (p != -1) {
            q = vertices[p][curr];

            if (length[p] + 1 == length[q]) {
                // we don't have to split q, just set the correct suffix link
                link[r] = q;
            } else {
                // we have to split, add q'
                vertices[verticesSize++] = vertices[q];
                length[lengthSize++] = length[p] + 1;
                link[linkSize++] = link[q]; // copy parent of q

                qq = verticesSize - 1;
                link[q] = qq;
                link[r] = qq;

                ++edgesCount;

                // move short classes pointing to q to point to q'
                while (p >= 0 && vertices[p][curr] == q) {
                    vertices[p][curr] = qq;
                    p = link[p];
                }

            }
        }

        last = r;
        ++i;
    }
}


//SuffixAutomaton::SuffixAutomaton(const std::string &s) {
//    unsigned newSize = s.size() * 2;
//    edges = new int *[newSize];
////    for (int i = 0; i < newSize; ++i) {
////        edges[i] = new int[26];
////        std::memset(edges[i], -1, 26 * sizeof(int));
////    }
//    link = new int[newSize];
//    length = new int[newSize];
////    edges.reserve(newSize);
////    link.reserve(newSize);
////    length.reserve(newSize);
//
//    // add the initial node
//    edges[edgesSize] = new int[26];
//    std::memset(edges[edgesSize++], -1, 26 * sizeof(int));
//    link[linkSize++] = -1;
//    length[lengthSize++] = 0;
////    edges.emplace_back(26, -1);
////    link.push_back(-1);
////    length.push_back(0);
//    last = 0;
//
//    char curr;
//    int r;
//    int p;
//    int q;
//    int qq;
//    for (int i = 0; i < s.size(); ++i) {
//        curr = s[i] - 97;
//
//        // construct r
//        edges[edgesSize] = new int[26];
//        std::memset(edges[edgesSize++], -1, 26 * sizeof(int));
//        length[lengthSize++] = i + 1;
//        link[linkSize++] = 0;
//
////        edges.emplace_back(26, -1);
////        length.push_back(i + 1);
////        link.push_back(0);
////        r = (int) edges.size() - 1;
//        r = edgesSize - 1;
//
//        // add edges to r and find p with link to q
//        p = last;
//        while (p >= 0 && edges[p][curr] == -1) {
//            edges[p][curr] = r;
//            p = link[p];
//
//            ++edgesCount;
//        }
//
//        if (p != -1) {
//            q = edges[p][curr];
//
//            if (length[p] + 1 == length[q]) {
//                // we don't have to split q, just set the correct suffix link
//                link[r] = q;
//            } else {
//                // we have to split, add q'
//                edges[edgesSize++] = edges[q];
//                length[lengthSize++] = length[p] + 1;
//                link[linkSize++] = link[q]; // copy parent of q
////                edges.push_back(edges[q]);
////                length.push_back(length[p] + 1);
////                link.push_back(link[q]); // copy parent of q
//
//                qq = edgesSize - 1;
////                qq = (int) edges.size() - 1;
//                // add qq as the new parent of q and r
//                link[q] = qq;
//                link[r] = qq;
//
//                ++edgesCount;
//
//                // move short classes pointing to q to point to q'
//                while (p >= 0 && edges[p][curr] == q) {
//                    edges[p][curr] = qq;
//                    p = link[p];
//                }
//
//            }
//        }
//
//        last = r;
//    }
//}

unsigned SuffixAutomaton::getStatesCount() const {
    return verticesSize;
//    return edges.size();
}

unsigned SuffixAutomaton::getTransitionsCount() const {
    return edgesCount;
}

unsigned SuffixAutomaton::getLongestSquaresCount() const {
    return squaresCount;
}