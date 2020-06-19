#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 9999

typedef struct node {
  unsigned int value;
  struct node *next;
} node;

node *create_node(int value, node *next);
void add_node(int value, node **head);
void append_node(node **last, int value);
void dealloc_list(node **head);
bool print_large_int_util(node *head);
void print_large_int(node *head);
void add_large_int(node *augend, node *addend, node **sum);
node *mult_util_add(node *part_prod, node *cur_prod, node *prev);
int mult_large_int(node *multiplicand, node *multiplier, node **product);
void flush_blanks();
bool read_large_int(node **head);
int eval_large_expr();
