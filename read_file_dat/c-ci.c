#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    float a;
    double b;
    int c;
    char d;
    FILE *fp;

    fp = fopen("will.dat", "r");

    if (fp == NULL)
    {
        printf("Error. Unable to read file.");
        // tell the shell there has been an error
        return 1;
    }

    fread(&a, sizeof(float), 1, fp);
    fread(&b, sizeof(double), 1, fp);
    fread(&c, sizeof(int), 1, fp);
    fread(&d, sizeof(char), 1, fp);

    fclose(fp);

    printf("\nFloat: %f\n", a);
    printf("Double: %f\n", b);
    printf("Int: %d\n", c);
    printf("Char: %c\n\n", d);

    return (0);
}