#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const int SIZE = 400000000;

int **states;
int *link;
int *length;
int last;

int state_size = 0;

int transition_count = 0;
int square_count = 0;

void build(istream &istream)
{
    states = new int *[SIZE];
    link = new int[SIZE];
    length = new int[SIZE];

    // add the initial node
    states[state_size] = new int[26];
    link[state_size] = -1;
    length[state_size] = 0;
    memset(states[state_size++], -1, 26 * sizeof(int));

    last = 0;

    char curr;
    int r;
    int p;
    int q;
    int qq;

    while (istream.get(curr) && curr != '\n')
    {
        curr -= 97;

        // construct r
        r = state_size;
        states[state_size] = new int[26];
        length[state_size] = length[last] + 1;
        link[state_size] = 0;
        memset(states[state_size++], -1, 26 * sizeof(int));

        // add edges to r and find p with link to q
        p = last;
        // while (p >= 0 && states[p][curr] == -1)
        while (p >= 0 && states[p][curr] == -1)
        {
            states[p][curr] = r;
            p = link[p];

            ++transition_count;
        }

        if (p != -1)
        {
            q = states[p][curr];

            if (length[p] + 1 == length[q])
            {
                // we don't have to split q, just set the correct suffix link
                link[r] = q;
            }
            else
            {
                // we have to split, add q'
                qq = state_size;
                int *copy = new int[26];
                for (int it = 0; it < 26; ++it)
                {
                    copy[it] = states[q][it];

                    if (copy[it] != -1)
                        ++transition_count;
                }

                states[state_size] = copy;
                length[state_size] = length[p] + 1;
                link[state_size++] = link[q]; // copy parent of q

                link[q] = qq;
                link[r] = qq;

                // move short classes pointing to q to point to q'
                while (p >= 0 && states[p][curr] == q)
                {
                    states[p][curr] = qq;
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
    build(input);
    printf("%d\n%d\n%d\n", state_size, transition_count, square_count);

    return 0;
}