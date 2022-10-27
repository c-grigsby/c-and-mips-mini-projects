/*******************************
* c1.c | Lab-C1 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void combine(int digit1, int digit2, int digit3, int *num, int *reversed_num)
{
    char temp_num[30]; 
    char temp_reversed_num[30]; 
    char *ptr;

    /*convert the ints to strings for manipulation*/
    sprintf(temp_num, "%d%d%d", digit1, digit2, digit3);
    sprintf(temp_reversed_num, "%d%d%d", digit3, digit2, digit1);
    /*convert the manipulated strings back to ints*/
    *num = strtol(temp_num, &ptr, 10);
    *reversed_num = strtol(temp_reversed_num, &ptr, 10);
}

int main(int argc, char **argv)
{
    int digit1;
    int digit2;
    int digit3;
    int num; 
    int reversed_num; 

    printf("\nEnter 3 digits seperated by spaces: ");
    scanf("%d %d %d", &digit1, &digit2, &digit3);

    if (digit1 > 9 || digit2 > 9 || digit3 > 9) {
        printf("\nError, arguments must single digits\n");
    }
    else if (digit1 < 0 || digit2 < 0 || digit3 < 0) {
        printf("\nError, arguments cannot be negative values\n");
    }
    else {
        combine(digit1, digit2, digit3, &num, &reversed_num);
        printf("\nNumber: %d\n", num); 
        printf("\nReversed Number: %d\n\n", reversed_num);
    }
    return 0;
}