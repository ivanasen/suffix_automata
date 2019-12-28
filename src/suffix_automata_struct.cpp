#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "shrinking_array.cpp"
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

using int_array = shrinking_array<26>;

struct State
{
    int_array states;
    int link;
    int length;

    State(int link, int length) : link(link), length(length)
    {
    }
};

const int SIZE = 400000000;

int final_count = 0;
int transition_count = 0;
int square_count = 0;

vector<State> states;

void mark_final_states(int last)
{
    while (last > -1)
    {
        ++final_count;
        last = states[last].link;
    }
}

void build(istream &istream)
{
    const int default_val = int_array::DEFAULT;
    states.reserve(SIZE);
    int last = 0;

    // add the initial node
    states.emplace_back(-1, 0);

    char curr_char;
    int r;
    int p;
    int q;
    int qq;

    while (istream.get(curr_char) && curr_char != '\n')
    {
        curr_char -= 97;

        r = states.size();
        states.emplace_back(0, states[last].length + 1);

        // add edges to r and find p with link to q
        p = last;
        while (p > default_val && states[p].states.get(curr_char) == default_val)
        {
            states[p].states.set(curr_char, r);
            p = states[p].link;
            ++transition_count;
        }

        if (p != default_val)
        {
            q = states[p].states.get(curr_char);

            if (states[p].length + 1 == states[q].length)
                // we don't have to split q, just set the correct suffix link
                states[r].link = q;
            else
            {
                // we have to split, add q'
                qq = states.size();
                states.push_back(states[q]);
                states.back().length = states[p].length + 1;

                states[r].link = qq;
                states[q].link = qq;

                transition_count += states.back().states.size;

                // move short classes pointing to q to point to q'
                while (p > default_val && states[p].states.get(curr_char) == q)
                {
                    states[p].states.set(curr_char, qq);
                    p = states[p].link;
                }
            }
        }

        last = r;
    }

    mark_final_states(last);
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
    printf("%lu\n%d\n%d\n", states.size(), transition_count, final_count);

    return 0;
}