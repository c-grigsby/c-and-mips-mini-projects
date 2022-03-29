#include <stdio.h>
#include <stdlib.h>

// printArray: displays the contents of an array to the terminal
void printArray(int* A, int len)
{
  printf("array = { ");
  for (int i = 0; i < len; ++i)
  {
      if (i == len-1) 
      {
        printf("%d ", A[i]); // then no comma
      } else {
        printf("%d, ", A[i]);
      }
  }
  printf("}\n");
}

// swap: helper method to sort 
void swap(int v[], int k)
{
  int temp;
  temp = v[k];
  v[k] = v[k+1];
  v[k+1] = temp;
}

// sort: sorts an array via bubbleSort O(n^2)
void sort (int v[], int n)
{
	  int i, j;
	  for (i = 0; i < n; i += 1) {
	    for (j = i - 1; j >= 0 && v[j] > v[j + 1]; j -= 1) {
	      swap(v,j);
	    }
	  }
}

int main(int argc, char** argv) 
{
  int arrLength = 8; 
  int A[8] = {3, 2, 5, 4, 7, 4, 8, 6};

  printArray(A, arrLength); 

  sort(A, arrLength);

  printArray(A, arrLength); 

  return 0; 
}
