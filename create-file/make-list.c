#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./make-list <num_terms> <output file name>\n");
    }
    else
    {
        printf("Number Of Arguments Passed: %d\n", argc);
    }
    return 0;
}