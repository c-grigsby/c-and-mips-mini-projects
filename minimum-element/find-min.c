#include <stdio.h>

void print_intArray(int arr[], int arrLength)
{
    printf("array = { ");
    for (int i = 0; i < arrLength; ++i)
    {
        if (i == arrLength - 1)
        {
            printf("%d ", arr[i]);
        }
        else
        {
            printf("%d, ", arr[i]);
        }
    }
    printf("}\n");
}

int main(void)
{
    int A[] = {3, -5, 3, 2, 1, 6, -9, 4};
    int A_length = sizeof A / sizeof A[0];
    int min = A[0];

    for (int i = 1; i < A_length; ++i)
    {
        int current = A[i];
        if (current < min)
        {
            min = current;
        }
    }
    print_intArray(A, A_length);
    printf("The minimum integer is: %d\n", min);
    return min;
}