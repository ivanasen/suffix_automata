#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>

#include "SuffixAutomata.h"

void build(char *in, SuffixAutomata &automata)
{
    int fileDesc = open(in, O_RDONLY);
    if (fileDesc == -1)
        perror("open");

    struct stat buf;
    fstat(fileDesc, &buf);
    int fileSize = buf.st_size;

    char *mmapedFile =
        (char *)mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fileDesc, 0);
    if (mmapedFile == MAP_FAILED)
        perror("map failed");

    for (int i = 0; i < fileSize; ++i)
        automata.addLetter(mmapedFile[i]);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        puts("No input provided.");
        return 0;
    }

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