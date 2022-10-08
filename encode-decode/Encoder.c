#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// decode_file: decodes a file to the provided model and write results to unencoded.txt
int decode_file(char *fileName) {
    // ensure the required file exists
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL) {
      printf("\nThe decode function requires a file named: %s", fileName);
      return -1;
    }
    // init solution arr
    int SIZE = 300;
    char solution_arr[SIZE][10];
    char default_value[] = " ";
    for (int i = 0; i < SIZE; i++){
      strcpy(solution_arr[i], default_value);
    }
    // decode file
    int i = 0;
    char str[10];
    while (fscanf(fp, "%s", str) == 1)
    {
      if (strcmp(str, "10") == 0) {
          strcpy(solution_arr[i],"a");
      } else if (strcmp(str, "11") == 0) {
          strcpy(solution_arr[i],"b");
      } else if (strcmp(str, "12") == 0) {
          strcpy(solution_arr[i],"c");
      } else if (strcmp(str, "13") == 0) {
          strcpy(solution_arr[i],"d");
      } else if (strcmp(str, "14") == 0) {
          strcpy(solution_arr[i],"e");
      } else if (strcmp(str, "15") == 0) {
          strcpy(solution_arr[i],"f");
      } else if (strcmp(str, "16") == 0) {
          strcpy(solution_arr[i],"g");
      } else if (strcmp(str, "17") == 0) {
          strcpy(solution_arr[i],"h");
      } else if (strcmp(str, "18") == 0) {
          strcpy(solution_arr[i],"i");
      } else if (strcmp(str, "19") == 0) {
          strcpy(solution_arr[i],"j");
      } else if (strcmp(str, "20") == 0) {
          strcpy(solution_arr[i],"k");
      } else if (strcmp(str, "21") == 0) {
          strcpy(solution_arr[i],"l");
      } else if (strcmp(str, "22") == 0) {
          strcpy(solution_arr[i],"m");
      } else if (strcmp(str, "23") == 0) {
          strcpy(solution_arr[i],"n");
      } else if (strcmp(str, "24") == 0) {
          strcpy(solution_arr[i],"o");
      } else if (strcmp(str, "25") == 0) {
          strcpy(solution_arr[i],"p");
      } else if (strcmp(str, "26") == 0) {
          strcpy(solution_arr[i],"q");
      } else if (strcmp(str, "27") == 0) {
          strcpy(solution_arr[i],"r");
      } else if (strcmp(str, "28") == 0) {
          strcpy(solution_arr[i],"s");
      } else if (strcmp(str, "29") == 0) {
          strcpy(solution_arr[i],"t");
      } else if (strcmp(str, "30") == 0) {
          strcpy(solution_arr[i],"u");
      } else if (strcmp(str, "31") == 0) {
          strcpy(solution_arr[i],"v");
      } else if (strcmp(str, "32") == 0) {
          strcpy(solution_arr[i],"w");
      } else if (strcmp(str, "33") == 0) {
          strcpy(solution_arr[i],"x");
      } else if (strcmp(str, "34") == 0) {
          strcpy(solution_arr[i],"y");
      } else if (strcmp(str, "35") == 0) {
          strcpy(solution_arr[i],"z");
      } else if (strcmp(str, "36") == 0) {
          strcpy(solution_arr[i]," ");
      } else if (strcmp(str, "37") == 0) {
          strcpy(solution_arr[i],".");
      } else if (strcmp(str, "38") == 0) {
          strcpy(solution_arr[i],"?");
      } else if (strcmp(str, "39") == 0) {
        strcpy(solution_arr[i],"!");
      }
      i++;
    }
    fclose(fp);
    // write to file
    FILE *fptr = fopen("unencoded.txt", "w");
    if (fptr == NULL) {
      printf("\nError trying to write the file");
      return -1;
    } 
    printf("\nDecoding file...");
    for (int i = 0; i < SIZE; i++) {
        fprintf(fptr, "%s", solution_arr[i]);
    }
    fclose(fptr);
    printf("\nDecoding complete. File name: unencoded.txt\n");
    return 0;
}

// encode_file: encodes a file based upon the provided model to a file named encoded.txt
int encode_file(char *fileName) {
    // ensure the required file exists
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL) {
      printf("\nThe encode function requires a file named: %s", fileName);
      return -1;
    }
    // init solution arr
    int SIZE = 300;
    char solution_arr[SIZE][10];
    char default_value[] = " ";
    for (int i = 0; i < SIZE; i++){
      strcpy(solution_arr[i], default_value);
    }
    // encode file
    int i = 0;
    char ch[10];
    while (fgets(ch, 2, fp) != NULL)
    {
      if (strcmp(ch, "a") == 0 || strcmp(ch, "A") == 0) {
          strcpy(solution_arr[i],"10");
      } else if (strcmp(ch, "b") == 0 || strcmp(ch, "B") == 0) {
          strcpy(solution_arr[i],"11");
      } else if (strcmp(ch, "c") == 0 || strcmp(ch, "C") == 0) {
          strcpy(solution_arr[i],"12");
      } else if (strcmp(ch, "d") == 0 || strcmp(ch, "D") == 0) {
          strcpy(solution_arr[i],"13");
      } else if (strcmp(ch, "e") == 0 || strcmp(ch, "E") == 0) {
          strcpy(solution_arr[i],"14");
      } else if (strcmp(ch, "f") == 0 || strcmp(ch, "F") == 0) {
          strcpy(solution_arr[i],"15");
      } else if (strcmp(ch, "g") == 0 || strcmp(ch, "G") == 0) {
          strcpy(solution_arr[i],"16");
      } else if (strcmp(ch, "h") == 0 || strcmp(ch, "H") == 0) {
          strcpy(solution_arr[i],"17");
      } else if (strcmp(ch, "i") == 0 || strcmp(ch, "I") == 0) {
          strcpy(solution_arr[i],"18");
      } else if (strcmp(ch, "j") == 0 || strcmp(ch, "J") == 0) {
          strcpy(solution_arr[i],"19");
      } else if (strcmp(ch, "k") == 0 || strcmp(ch, "K") == 0) {
          strcpy(solution_arr[i],"20");
      } else if (strcmp(ch, "l") == 0 || strcmp(ch, "L") == 0) {
          strcpy(solution_arr[i],"21");
      } else if (strcmp(ch, "m") == 0 || strcmp(ch, "M") == 0) {
          strcpy(solution_arr[i],"22");
      } else if (strcmp(ch, "n") == 0 || strcmp(ch, "N") == 0) {
          strcpy(solution_arr[i],"23");
      } else if (strcmp(ch, "o") == 0 || strcmp(ch, "O") == 0) {
          strcpy(solution_arr[i],"24");
      } else if (strcmp(ch, "p") == 0 || strcmp(ch, "P") == 0) {
          strcpy(solution_arr[i],"25");
      } else if (strcmp(ch, "q") == 0 || strcmp(ch, "Q") == 0) {
          strcpy(solution_arr[i],"26");
      } else if (strcmp(ch, "r") == 0 || strcmp(ch, "R") == 0) {
          strcpy(solution_arr[i],"27");
      } else if (strcmp(ch, "s") == 0 || strcmp(ch, "S") == 0) {
          strcpy(solution_arr[i],"28");
      } else if (strcmp(ch, "t") == 0 || strcmp(ch, "T") == 0) {
          strcpy(solution_arr[i],"29");
      } else if (strcmp(ch, "u") == 0 || strcmp(ch, "U") == 0) {
          strcpy(solution_arr[i],"30");
      } else if (strcmp(ch, "v") == 0 || strcmp(ch, "V") == 0) {
          strcpy(solution_arr[i],"31");
      } else if (strcmp(ch, "w") == 0 || strcmp(ch, "W") == 0) {
          strcpy(solution_arr[i],"32");
      } else if (strcmp(ch, "x") == 0 || strcmp(ch, "X") == 0) {
          strcpy(solution_arr[i],"33");
      } else if (strcmp(ch, "y") == 0 || strcmp(ch, "Y") == 0) {
          strcpy(solution_arr[i],"34");
      } else if (strcmp(ch, "z") == 0 || strcmp(ch, "Z") == 0) {
          strcpy(solution_arr[i],"35");
      } else if (strcmp(ch, " ") == 0) {
          strcpy(solution_arr[i],"36");
      } else if (strcmp(ch, ".") == 0) {
          strcpy(solution_arr[i],"37");
      } else if (strcmp(ch, "?") == 0) {
          strcpy(solution_arr[i],"38");
      } else if (strcmp(ch, "!") == 0) {
        strcpy(solution_arr[i],"39");
      }
      i++;
    }
    fclose(fp);
    // write to file
    FILE *fptr = fopen("encoded.txt", "w");
    if (fptr == NULL) {
      printf("\nError trying to write the file");
      return -1;
    } 
    printf("\nEncoding file...");
    for (int i = 0; i < SIZE; i++) {
      fprintf(fptr, "%s ", solution_arr[i]);
    }
    fclose(fptr);
    printf("\nEncoding complete. File name: encoded.txt\n");
    return 0;
}

int main(int argc, char** argv)
{
    char menu_selection[30];
    char unencoded_file_name[] = "unencoded.txt";
    char encoded_file_name[] = "encoded.txt";

    printf("\nWelcome to the Encoder/Decoder program!\n\n");
    printf("To decode a file enter: d\n");
    printf("To encode a file enter: e\n");
    printf("Please enter your selection: ");
    scanf("%s", &menu_selection);

    if (strcmp(menu_selection, "d") == 0) {
      decode_file(encoded_file_name);
    } else if (strcmp(menu_selection, "e") == 0) {
      encode_file(unencoded_file_name);
    } else {
      printf("Error, Please enter a valid selection (d or e)");
    }
    printf("\nShutting down...\n");
    return 0;
}