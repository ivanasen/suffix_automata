#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

#include "SuffixAutomata.h"

const unsigned int BUFF_SIZE = 1 << 20;

void build(char *in, SuffixAutomata &automata)
{
    int file_desc = open(in, O_RDONLY);

    struct stat buf;
    fstat(file_desc, &buf);

    if (file_desc == -1)
        perror("open");

    ssize_t n = 0;
    char *buffer = new char[BUFF_SIZE];
    do
    {
        n = read(file_desc, buffer, BUFF_SIZE);
        for (int i = 0; i < n; ++i)
        {
            if (buffer[i] == '\n')
                break;
            automata.addLetter(buffer[i]);
        }
    } while (n > 0);
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    SuffixAutomata automata;
    build(argv[1], automata);
    printf("%d\n%d\n%d\n%d\n",
           automata.getStatesCount(),
           automata.getTransitionsCount(),
           automata.getFinalsCount(),
           automata.getSquaresCount());

    return 0;
}