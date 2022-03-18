#include <stdio.h>
#include <stdlib.h>

// Returns a copy of a string from a char array
void myStrCopy(char x[], char y[])
{
  int i; 
  i = 0; 
  while ((x[i]=y[i]) != '\0')
  {
    i += 1; 
  }
}

// printAllChars: Prints all the characters of a string in memory (past the null
// terminating character if present) this is interesting to review if the destination
// string is longer in length than the source string to be copied 
void printAllChars(char* a, int len){
  for(int i = 0; i < len; i++)
  {
    printf("%c", a[i]);   
  }
  printf("\n"); 
}

int main(int argc, char** argv) {
  char src[] = "ABCD";  
  char dest[] = "EFGHIJKLMNO";

  printf("%s\n", src);
  printf("%s\n", dest);
  printAllChars(dest, 12); 
  printf("\n"); 

  myStrCopy(dest, src);

  printf("%s\n", src);
  printf("%s\n", dest);
  printAllChars(dest, 12);
  printf("\n"); 
  
  return 0;
}