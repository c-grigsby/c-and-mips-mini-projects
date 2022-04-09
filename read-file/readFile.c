
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read_file: stores the contents of a text file into a string
char *read_file(char *fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r");

    if (fp == NULL)
        return NULL;

    // determine the number of characters in the file
    fseek(fp, 0, SEEK_END);
    // store the length of the file (num of chars)
    int length = ftell(fp);
    // move the file pointer back to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // dynamically allocate space of the character array that will store the string,
    // length plus one for the null terminating character
    char *string = malloc(sizeof(char) * (length + 1));

    char c;
    int i = 0;

    // while the character stored into c != end of file
    while ((c = fgetc(fp)) != EOF)
    {
        string[i] = c;
        i++;
    }
    // set last character at in the string to the null terminating character
    string[i] = '\0';

    fclose(fp);
    return string;
}

int main(int argc, char **argv)
{

    char fileName[1024];

    // prompt user for the file name
    printf("Enter file name: ");
    // store into the char fileName
    scanf("%s", fileName);

    char *file_contents = read_file(fileName);

    if (file_contents == NULL)
    {
        printf("Error. Unable to read file.");
        // tell the shell there has been an error
        return 1;
    }

    printf("\n=== File Contents ===\n%s\n\n", file_contents);

    // make space available again in dynamically allocated memory
    free(file_contents);

    return 0;
}
