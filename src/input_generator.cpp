#include <iostream>
#include <fstream>

using namespace std;

char random_char()
{
    return 97 + rand() % 26;
}

void random_string(ostream &os, int size)
{
    for (int i = 0; i < size; ++i)
        os << random_char();
}

int main()
{
    string file_name;
    int size;

    cin >> file_name >> size;
    ofstream output(file_name);

    random_string(output, size);
    puts("Done.");

    return 0;
}
