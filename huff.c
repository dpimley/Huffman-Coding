#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main(int argc, char * * argv){
  if (argc != 2){
    printf("ERROR: Incorrect number of inputs. Usage is huff <input file>.\n");
    return EXIT_FAILURE;
  }

  FILE * inf = fopen(argv[1], "r");

  if (inf == NULL){
    printf("ERROR: Input file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  long * freq_count = calloc(ASCII_COUNT, sizeof(long));

  read_file(inf, freq_count);

  fclose(inf);
  return EXIT_SUCCESS;
}

void read_file(FILE * infile, long * char_count){
  int c;
  while (1){
    c = fgetc(infile);
    if (c == EOF){
      break;
    char_count[c]++;
  }
}  
