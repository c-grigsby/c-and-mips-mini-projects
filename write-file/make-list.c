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
        char *user_num;
        char *file_name;
        FILE *fptr;

        user_num = argv[1];
        file_name = argv[2];

        fptr = fopen(file_name, "w");
        for (int i = 0; i < atoi(user_num); ++i)
        {
            fprintf(fptr, "%d\n", i);
        }
        fclose(fptr);
    }
    return 0;
}