#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#include "shrinking_array.cpp"

using namespace std;

struct State
{
    shrinking_array<26> states;
    int link;
    int length;

    State(int link, int length) : link(link), length(length)
    {
    }
};

const unsigned int SIZE = 400000000;
const unsigned int BUFF_SIZE = 1 << 20;
int state_count = 0;
int transition_count = 0;
int final_count = 0;
int square_count = 0;

int last;

int mark_final_states(vector<State> &states)
{
    int final_count = 0;
    while (last > -1)
    {
        ++final_count;
        last = states[last].link;
    }
    return final_count;
}

void add_letter(vector<State> &states, char c)
{
    c -= 97;

    int r = states.size();
    states.emplace_back(0, states[last].length + 1);

    // add edges to r and find p with link to q
    int p = last;
    while (p > -1 && states[p].states.get(c) == -1)
    {
        states[p].states.set(c, r);
        p = states[p].link;
        ++transition_count;
    }

    if (p != -1)
    {
        int q = states[p].states.get(c);

        if (states[p].length + 1 == states[q].length)
            // we don't have to split q, just set the correct suffix link
            states[r].link = q;
        else
        {
            // we have to split, add q'
            int qq = states.size();
            states.push_back(states[q]);
            states.back().length = states[p].length + 1;

            states[r].link = qq;
            states[q].link = qq;

            transition_count += states.back().states.size;

            // move short classes pointing to q to point to q'
            while (p > -1 && states[p].states.get(c) == q)
            {
                states[p].states.set(c, qq);
                p = states[p].link;
            }
        }
    }

    last = r;
}

void build(char *in)
{
    int file_desc = open(in, O_RDONLY);

    struct stat buf;
    fstat(file_desc, &buf);

    if (file_desc == -1)
        perror("open");

    vector<State> states;
    states.reserve(SIZE);

    // add the initial node
    states.emplace_back(-1, 0);

    ssize_t n = 0;
    char *buffer = new char[BUFF_SIZE];
    do
    {
        n = read(file_desc, buffer, BUFF_SIZE);
        for (int i = 0; i < n; ++i)
        {
            if (buffer[i] == '\n')
                break;
            add_letter(states, buffer[i]);
        }
    } while (n > 0);
    // delete[] buffer;

    state_count = states.size();
    final_count = mark_final_states(states);
}

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    build(argv[1]);
    printf("%d\n%d\n%d\n%d\n", state_count, transition_count, final_count, square_count);

    return 0;
}