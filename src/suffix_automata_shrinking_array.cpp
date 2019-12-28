#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "shrinking_array.cpp"
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const int SIZE = 400000000;

int state_size = 0;
int final_count = 0;
int transition_count = 0;
int square_count = 0;

void mark_final_states(int *link, int last)
{
    int curr = last;
    while (curr > -1)
    {
        ++final_count;
        curr = link[curr];
    }
}

void build(istream &istream)
{
    const int default_val = shrinking_array<26>::DEFAULT;
    shrinking_array<26> **states = new shrinking_array<26> *[SIZE];
    int *link = new int[SIZE];
    int *length = new int[SIZE];
    int last;

    // add the initial node
    states[state_size] = new shrinking_array<26>();
    link[state_size] = default_val;
    length[state_size++] = 0;

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
        states[state_size] = new shrinking_array<26>();
        length[state_size] = length[last] + 1;
        link[state_size++] = 0;

        // add edges to r and find p with link to q
        p = last;
        while (p > default_val && states[p]->get(curr) == default_val)
        {
            states[p]->set(curr, r);
            p = link[p];

            ++transition_count;
        }

        if (p != default_val)
        {
            q = states[p]->get(curr);

            if (length[p] + 1 == length[q])
            {
                // we don't have to split q, just set the correct suffix link
                link[r] = q;
            }
            else
            {
                // we have to split, add q'
                qq = state_size;
                shrinking_array<26> *copy = new shrinking_array<26>(*states[q]);
                states[state_size] = copy;
                length[state_size] = length[p] + 1;
                link[state_size++] = link[q];

                link[r] = qq;
                link[q] = qq;

                transition_count += copy->size;

                // move short classes pointing to q to point to q'
                while (p > default_val && states[p]->get(curr) == q)
                {
                    states[p]->set(curr, qq);
                    p = link[p];
                }
            }
        }

        last = r;
    }

    mark_final_states(link, last);
}

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    ifstream input(argv[1]);
    // stringstream ss;
    // ss << "abcbc";
    // build(ss);
    build(input);
    printf("%d\n%d\n%d\n", state_size, transition_count, final_count);

    return 0;
}