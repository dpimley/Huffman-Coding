#ifndef HUFF_H
#define HUFF_H

#define ASCII_COUNT 256

typdef struct t_node{
  int count;
  int label;
  struct t_node * left;
  struct t_node * right;
}t_node;
