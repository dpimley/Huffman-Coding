#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char * * argv){

  //Return error if there is more than 1 inputs

  if (argc != 2){
    printf("ERROR: Incorrect number of inputs. Usage is huff <input file>.\n");
    return EXIT_FAILURE;
  }

  //Create input file pointer, return if NULL

  FILE * inf = fopen(argv[1], "r");

  if (inf == NULL){
    printf("ERROR: Input file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  //create array (initialized to 0) to hold frequency of characters read in

  long * freq_count = calloc(ASCII_COUNT, sizeof(long));

  //read in file and populate frequency count array

  read_file(inf, freq_count);

  //initialize last element (pseudo eof) to have frequency 1
 
  freq_count[256] = 1;

  //create the initial priority queue that holds pointers to t_node trees 
 
  p_queue * heap_head = createHeap(freq_count);

  //creates the huffman tree

  t_node * huff_tree = build_huff_tree(heap_head);

  //builds the output file string name and opens the file, if the file is NULL return error

  char * out_string = strcat(argv[1], ".huff");

  FILE * outf = fopen(out_string, "w");

  if (outf == NULL){
    printf("ERROR: Output file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  //prints the header of the compressed file

  print_header(huff_tree, outf, 0);

  //creates a table of codes for each character present in the input file

  bit_code * code_table = malloc(sizeof(bit_code) * ASCII_COUNT);

  create_huff_table(code_table, huff_tree, 0, 0);

  //int i;
  //for (i = 0; i < ASCII_COUNT; i++){
  //  if (freq_count[i] != 0){
  //    if (i == 256){
  //      printf("EOF: %d, %d\n", code_table[i].code, code_table[i].length);
  //   }
  //   else{
  //      printf("%c: %d, %d\n", i, code_table[i].code, code_table[i].length);
  //    }
  //  }
  //}

  fseek(inf, 0, SEEK_SET);

  //write out compressed data to output file

  write_compressed_data(inf, outf, code_table);

  free(freq_count);
  free(huff_tree);
  free(code_table);
  fclose(inf);
  fclose(outf);
  return EXIT_SUCCESS;
}

//function reads until EOF and populates the frequency count array

void read_file(FILE * infile, long * char_count){
  int c;
  while (1){
    c = fgetc(infile);
    if (c == EOF)
      break;
    char_count[c]++;
  }
  return;
}

//function given a count and a label creates a t_node and returns a pointer to it

t_node * create_t_node(long count, int label){
  t_node * node = malloc(sizeof(t_node));
  node->count = count;
  node->label = label;
  return node;
}

//function inserts a t_node struct in the proper location in the priority queue

void insert_heap(p_queue * heap_head, t_node * ins_node){
  if (heap_head->size == 0){
    heap_head->t_arr[0] = ins_node;
    heap_head->size++;
  }
  else{
    heap_head->size++;
    heap_head->t_arr[heap_head->size - 1] = ins_node;
    int child_idx = heap_head->size - 1;
    int parent_idx = (child_idx - 1) / 2;
    while (heap_head->t_arr[parent_idx]->count > ins_node->count && child_idx > 0){
      heap_head->t_arr[child_idx] = heap_head->t_arr[parent_idx];
      child_idx = parent_idx;
      parent_idx = (child_idx - 1) / 2;
    }
    heap_head->t_arr[child_idx] = ins_node;
  }
  return;
}

//function creates an initial priority queue based on the frequency counts of the read chars

p_queue * createHeap(long * freq){
  p_queue * heap_head = malloc(sizeof(p_queue));
  heap_head->t_arr = malloc(ASCII_COUNT * sizeof(t_node *));
  heap_head->size = 0;
  int i;
  for (i = 0; i < ASCII_COUNT; i++){
    if (freq[i]){
      t_node * ins_node = create_t_node(freq[i], i);
      insert_heap(heap_head, ins_node);
    }
  }
  return heap_head;
}

//function removes the minimum from the priority queue

t_node * remove_min(p_queue * heap_head){
  if (heap_head->size > 0){
    t_node * min_element = heap_head->t_arr[0];
    t_node * last_element = heap_head->t_arr[heap_head->size - 1];
    heap_head->t_arr[heap_head->size - 1] = NULL;
    heap_head->t_arr[0] = last_element;
    heap_head->size--;
    downward_heapify(heap_head->t_arr, heap_head->size, 0);
    return min_element;
  }
  return 0;
}

//function puts head of priority queue into correct place within heap

void downward_heapify(t_node * * t_arr, int size, int root){
  int left_idx = (2 * root) + 1;
  int right_idx = (2 * root) + 2;
  t_node * min_node = NULL;
  if (left_idx >= size && right_idx >= size){
    return;
  }
  if (left_idx < size && t_arr[root]->count <= t_arr[left_idx]->count){
    if (right_idx < size && t_arr[root]->count <= t_arr[right_idx]->count){
      return;
    }
  }
  if (right_idx < size && t_arr[right_idx]->count <= t_arr[left_idx]->count){
    min_node = t_arr[right_idx];
    t_arr[right_idx] = t_arr[root];
    t_arr[root] = min_node;
    downward_heapify(t_arr, size, right_idx);
  }
  else{
    min_node = t_arr[left_idx];
    t_arr[left_idx] = t_arr[root];
    t_arr[root] = min_node;
    downward_heapify(t_arr, size, left_idx);
  }
 return; 
}

//function takes in the head of a priority queue and returns a pointer to a huffman tree

t_node * build_huff_tree(p_queue * heap_head){
  while (heap_head->size != 1){
    t_node * tmp_left = remove_min(heap_head);
    t_node * tmp_right = remove_min(heap_head);
    t_node * top = create_t_node(tmp_left->count + tmp_right->count, 42);

    top->left = tmp_left;
    top->right = tmp_right;

    insert_heap(heap_head, top);
  }
  return heap_head->t_arr[0];
}

//function prints the header of the compressed file (preorder traversal)

void print_header(t_node * head, FILE * outfile, int depth){
  if (head->left == NULL && head->right == NULL){
    if (head->label != 256){
      fputc(49, outfile);
      fputc(head->label, outfile);
    }
    else if (head->label == 256){
      fputc(50, outfile);
    }
    return;
  }

  fputc(48, outfile);

  print_header(head->left, outfile, depth + 1);
  print_header(head->right, outfile, depth + 1);

  if (!depth)
    fputc(48, outfile);
  return;
}

//function creates a table of huffman codes values

void create_huff_table(bit_code * huff_table, t_node * head, int cur_path, int depth){
  if (head->left == NULL && head->right == NULL){
    huff_table[head->label].code = cur_path;
    huff_table[head->label].length = depth;
    return;
  }
  int cur_left = (cur_path << 1) | 0;
  int cur_right = (cur_path << 1) | 1;

  create_huff_table(huff_table, head->right, cur_right, depth + 1);
  create_huff_table(huff_table, head->left, cur_left, depth + 1);
  return;
}

//function converts each character from an input file into its respective huffman value

void write_compressed_data(FILE * infile, FILE * outfile, bit_code * huff_table){
  int c;
  int buffer = 0, number_added = 0;
  while(1){
    c = fgetc(infile);
    if (c == EOF){
      int len_eof = huff_table[256].length;
      int eof_code = huff_table[256].code;
      while (len_eof > 0){
        int off_eof = (eof_code >> (len_eof - 1)) & 1;
        buffer = (buffer << 1) | off_eof;
        number_added++;
        len_eof--;
        if (number_added == 8){
          fputc(buffer, outfile);
          number_added = 0;
          buffer = 0;
        }
      }
      break;
    }
    else{
      int len_code = huff_table[c].length;
      while (len_code > 0){
        int char_code = huff_table[c].code;
        int offset = (char_code >> (len_code - 1)) & 1;
        buffer = (buffer << 1) | offset;
        number_added++;
        len_code--;
        if (number_added == 8){
          fputc(buffer, outfile);
          number_added = 0;
          buffer = 0;
        }
      }
    }    
  }
  if (number_added == 0){
    return;
  }
  else{
    buffer = buffer << (8 - number_added);
    fputc(buffer, outfile);
    return;
  }
}
