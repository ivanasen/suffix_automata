#include <iostream>
#include <fstream>

using namespace std;

char randomChar()
{
    return 97 + rand() % 26;
}

void randomString(ostream &os, int size)
{
    for (int i = 0; i < size; ++i)
        os << randomChar();
}

int main()
{
    string file_name;
    int size;

    cin >> file_name >> size;
    ofstream output(file_name);

    randomString(output, size);
    puts("Done.");

    return 0;
}
