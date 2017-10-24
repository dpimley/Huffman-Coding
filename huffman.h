#ifndef HUFF_H
#define HUFF_H
//257 to include all ascii characters and the pseudo-EOF character
#define ASCII_COUNT 257

typedef struct t_node{
  long count;
  int label;
  struct t_node * left;
  struct t_node * right;
}t_node;

typedef struct p_queue{
  t_node * * t_arr;
  int head;
  int size;
}p_queue;

typedef struct bit_code{
  int code;
  int length;
}bit_code;

void read_file(FILE * infile, long * char_count);

t_node * create_t_node(long count, int label);

void insert_heap(p_queue * heap_head, t_node * ins_node);

p_queue * createHeap(long * freq);

t_node * remove_min(p_queue * heap_head);

void downward_heapify(t_node * * t_arr, int size, int root);

t_node * build_huff_tree(p_queue * heap_head);

void print_header(t_node * head, FILE * outfile);

void create_huff_table(bit_code * huff_table, t_node * head, int cur_path, int depth);

void write_compressed_data(FILE * infile, FILE * outfile, bit_code * huff_table);



void read_header(t_node * * head, FILE * infile);

void print_pre_order(t_node * head);

void print_file(FILE * infile, FILE * outfile, t_node * head);
#endif
