#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./make-list <num_terms> <output file name>\n");
    }
    else
    {
        char *file_name;
        FILE *fptr;

        file_name = argv[2];
        fptr = fopen(file_name, "w");
        fprintf(fptr, "Hello World");

        fclose(fptr);
    }
    return 0;
}