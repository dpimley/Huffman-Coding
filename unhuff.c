#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char * * argv){
  if (argc != 2){
    printf("ERROR: Incorrect number of inputs. Usaage is unhuff <input file>.\n");
    return EXIT_FAILURE;
  }

  FILE * inf = fopen(argv[1], "r");
  
  if (inf == NULL){
    printf("ERROR: Input file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  t_node * huff_tree = NULL;

  read_header(&huff_tree, inf);

  //print_pre_order(huff_tree);

  int c = fgetc(inf);

  if (c != '0'){
    printf("ERROR in HUFFMAN HEADER\n");
  }
  
  strcat(argv[1], ".unhuff");

  FILE * outf = fopen(argv[1], "w");

  if (outf == NULL){
    printf("ERROR: Output file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  print_file(inf, outf, huff_tree);

  free(huff_tree);
  fclose(inf);
  fclose(outf);
 
  return EXIT_SUCCESS;
}

t_node * create_t_node(long count, int label){
  t_node * node = malloc(sizeof(t_node));
  node->count = count;
  node->label = label;
  return node;
}

//function reads the header and creates a huffman tree equivalent to the one used in huff

void read_header(t_node * * head, FILE * infile){
  unsigned char c;
  fread(&c, 1, 1, infile);
  if (c == '0'){
    t_node * connect_node = create_t_node(0, 0);
    (*head) = connect_node;
    read_header(&((*head)->left), infile);
    read_header(&((*head)->right), infile);
    return;
  }
  else if (c == '1' || c == '2'){
    t_node * leaf_node = NULL;
    if (c == '2'){
      leaf_node = create_t_node(0, 256);
    }
    else if (c == '1'){
      fread(&c, 1, 1, infile);
      leaf_node = create_t_node(0, c);
    }
    *head = leaf_node;
    return;
  }
}

//function helps debug read_header by printing the preorder traversal of the tree

void print_pre_order(t_node * head){
  if (head == NULL){
    return;
  }

  if (head->left == NULL && head->right == NULL){
    if (head->label == 256){
      printf("LABEL: EOF\n");
    }
    else{
      printf("LABEL: %c\n", head->label);
    }
  }
  print_pre_order(head->left);
  print_pre_order(head->right);

  return;
}

//function that reads in the input file and prints out the ascii char the huffman code represents

void print_file(FILE * infile, FILE * outfile, t_node * head){
  int buffer = 0, total = 0, eof_flag = 0, dir = 0;
  t_node * cur = head;
  while (!eof_flag){
    if (total == 0){
      fread(&buffer, 1, 1, infile);
      total = 8;
    }
    if (cur->left == NULL && cur->right == NULL){
      if (cur->label == 256){
        eof_flag = 1;
      }
      else{
        fputc(cur->label, outfile);
        cur = head;
      }
    }
    dir = (buffer >> (total - 1)) & 1;
    if (dir == 0){
      cur = cur->left;
    }
    else if (dir == 1){
      cur = cur->right;
    }
    total--;
  } 
 return;
}
