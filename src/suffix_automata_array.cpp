#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const long SIZE = 400000000;

long state_size = 1;
long transition_count = 0;
long square_count = 0;

void build(istream &istream)
{
    int *states = new int[SIZE * 26 + 26];
    int *link = new int[SIZE + 1];
    int *length = new int[SIZE + 1];
    long last;

    // add the initial node
    link[state_size] = 0;
    length[state_size++] = 0;

    last = 1;

    char curr;
    long r;
    long p;
    long q;
    long qq;

    while (istream.get(curr) && curr != '\n')
    {
        curr -= 97;

        // construct r
        r = state_size;
        length[state_size] = length[last] + 1;
        link[state_size++] = 1;

        // add edges to r and find p with link to q
        p = last;
        while (p > 0 && states[p * 26 + curr] == 0)
        {
            states[p * 26 + curr] = r;
            p = link[p];

            ++transition_count;
        }

        if (p != 0)
        {
            q = states[p * 26 + curr];

            if (length[p] + 1 == length[q])
            {
                // we don't have to split q, just set the correct suffix link
                link[r] = q;
            }
            else
            {
                // we have to split, add q'
                qq = state_size;
                for (char it = 0; it < 26; ++it)
                {
                    states[qq * 26 + it] = states[q * 26 + it];

                    if (states[qq * 26 + it] != 0)
                        ++transition_count;
                }
                length[state_size] = length[p] + 1;
                link[state_size++] = link[q]; // copy parent of q

                link[q] = qq;
                link[r] = qq;

                // move short classes pointing to q to point to q'
                while (p > 0 && states[p * 26 + curr] == q)
                {
                    states[p * 26 + curr] = qq;
                    p = link[p];
                }
            }
        }

        last = r;
    }
}

int main(int argc, char **argv)
{
    ifstream input(argv[1]);
    // stringstream ss;
    // ss << "abcbc";
    // build(ss);
    build(input);
    printf("%d\n%d\n%d\n", (int) state_size - 1, (int) transition_count, (int) square_count);

    return 0;
}