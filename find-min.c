#include <stdio.h>

// print method to display an int array
void print_intArray(int arr[], int arrLength)
{
    printf("array = { ");
    for (int i = 0; i < arrLength; ++i)
    {
        if (i == arrLength - 1) // no comma needed
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
    int A[] = {3, 5, 3, 2, 1, 6, 9, 4};
    int A_length = sizeof A / sizeof A[0];
    int min = A[0]; // init a min, first element of the arr

    for (int i = 1; i < A_length; ++i) // traverse starting at i=1
    {
        int current = A[i];
        if (current < min) // compare if A[i] is less than min
        {
            min = current; // update min as needed
        }
    }
    print_intArray(A, A_length);
    printf("The minimum integer is: %d\n", min);
    return min;
}