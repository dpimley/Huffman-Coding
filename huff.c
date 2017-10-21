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
  return;
}

t_node * create_t_node(long count, int label){
  t_node * node = malloc(sizeof(t_node));
  t_node->count = count;
  t_node->label = label;
  return t_node;
}

void insert_min_heap(p_queue * heap_head, t_node * ins_node){
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

p_queue * createHeap(long * freq){
  p_queue * heap_head = malloc(sizeof(p_queue));
  heap_head->t_arr = malloc(ASCII_COUNT * sizeof(* t_node));
  int i;
  for (i = 0; i < ASCII_COUNT; i++){
    if (freq[i]){
      t_node * ins_node = create_t_node(freq[i], i);
      insert_min_heap(heap_head, ins_node);
    }
  }
  return heap_head;
}

t_node * remove_min(p_queue * heap_head)
  if (heap_head->size > 0){
    min_element = heap_head->t_arr[0];
    last_element = heap_head->t_arr[heap_head->size - 1];
    heap_head->t_arr[heap_head->size - 1] = NULL;
    heap_head->t_arr[0] = last_element;
    downward_heapify(heap_head->t_arr, heap_head->size);
    heap_head->size--;
    return min_element;

void downward_heapify(t_node * * t_arr, int size){
  t_node * temp = t_arr[0];
  int i = 1;
  while(i < size / 2){
    int j = 2 * i;
    if (j < n and t_arr[j - 1]->count < t_arr[j]->count){
      j += 1;
    }
    else if (temp->count >= t_arr[j - 1]){
      break;
    }
    else{
      t_arr[i - 1] = t_arr[j - 1]
      i = j;
    }
  t_arr[i - 1] = temp;
  return;
}
