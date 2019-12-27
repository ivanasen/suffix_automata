#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const int SIZE = 400000000;

int **states;
int *link;
int *length;
int last;

int state_size = 0;
int link_size = 0;
int length_size = 0;

int transition_count = 0;
int square_count = 0;

void build(istream &istream)
{
    states = new int *[SIZE];
    link = new int[SIZE];
    length = new int[SIZE];

    // add the initial node
    states[state_size] = new int[26];
    memset(states[state_size++], -1, 26 * sizeof(int));
    link[link_size++] = -1;
    length[length_size++] = 0;
    last = 0;

    char curr;
    int r;
    int p;
    int q;
    int qq;

    int i = 0;

    while (istream.get(curr) && curr != '\n')
    {
        curr -= 97;

        // construct r
        states[state_size] = new int[26];
        memset(states[state_size++], -1, 26 * sizeof(int));
        length[length_size++] = i + 1;
        link[link_size++] = 0;
        r = state_size - 1;

        // add edges to r and find p with link to q
        p = last;
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
                states[state_size++] = states[q];
                length[length_size++] = length[p] + 1;
                link[link_size++] = link[q]; // copy parent of q

                qq = state_size - 1;
                link[q] = qq;
                link[r] = qq;

                ++transition_count;

                // move short classes pointing to q to point to q'
                while (p >= 0 && states[p][curr] == q)
                {
                    states[p][curr] = qq;
                    p = link[p];
                }
            }
        }

        last = r;
        ++i;
    }
}

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    ifstream input(argv[1]);
    build(input);
    printf("%d\n%d\n%d\n", state_size, transition_count, square_count);

    return 0;
}