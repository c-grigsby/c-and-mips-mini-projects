#include <stdio.h>

void print(int A[], int length)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%d\t", A[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {6, 7, 8, 9, 10};
    int c[5];

    for (int i = 0; i < 5; ++i)
    {
        c[i] = a[i] + b[i];
    }
    print(c, 5);
    return 0;
}