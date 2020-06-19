#include "large_int_header.h"

node *create_node(int value, node *next) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->value = value;
  tmp->next = next;
  return tmp;
}

void add_node(int value, node **head) {
  node *tmp = create_node(value,*head);
  *head = tmp;
}

void append_node(node **last, int value) {
  if (*last == NULL) {
    (*last) = create_node(value,NULL);
    return;
  }
  (*last)->next = create_node(value,NULL);
  (*last) = (*last)->next;
  return;
}

void dealloc_list(node **head) {
  node *tmp;
  while (*head != NULL) {
    tmp = (*head)->next;
    free(*head);
    *head = tmp;
  }
}

bool print_large_int_util(node *head) {
  if (head == NULL)
    return false;

  int tmp, i;
  bool is_head = true, non_zero = false;
  if (head->next != NULL) {
    is_head = false;
    non_zero = print_large_int_util(head->next);

    if (non_zero) {
      printf(",");
      for (tmp = head->value * 10, i = 0; tmp <= MAX && i < 3; tmp *= 10, i++)
        printf("0");
    }
  }
  if (is_head && head->value)
    non_zero = true;
  if (non_zero || is_head)
    printf("%d", head->value);
  return non_zero;
}

void print_large_int(node *head) {
  print_large_int_util(head);
  printf("$\n");
}

void add_large_int(node *augend, node *addend, node **sum) {
  node *tmp, *prev = NULL, *rest;
  int carry = 0, result;
  while (augend != NULL && addend != NULL) {
    result = addend->value + augend->value + carry;
    carry = result / (MAX + 1);
    result = result % (MAX + 1);
    tmp = (node *)malloc(sizeof(node));
    tmp->value = result;
    tmp->next = NULL;
    if (prev == NULL)
      *sum = tmp;
    else
      prev->next = tmp;
    prev = tmp;
    augend = augend->next;
    addend = addend->next;
  }
  rest = (augend != NULL ? augend : addend);
  while (rest != NULL) {
    result = rest->value + carry;
    carry = result / (MAX + 1);
    result = result % (MAX + 1);
    tmp = (node *)malloc(sizeof(node));
    tmp->value = result;
    tmp->next = NULL;
    if (prev == NULL)
      *sum = tmp;
    if (prev != NULL)
      prev->next = tmp;

    prev = tmp;
    rest = rest->next;
  }
  if (carry) {
    tmp = (node *)malloc(sizeof(node));
    tmp->value = carry;
    tmp->next = NULL;
    prev->next = tmp;
  }
  return;
}

node *mult_util_add(node *part_prod, node *cur_prod, node *prev) {
  node *new_part_prod = NULL;
  add_large_int(part_prod, cur_prod, &new_part_prod);
  dealloc_list(&part_prod);
  dealloc_list(&cur_prod);
  if (prev != NULL)
    prev->next = new_part_prod;
  prev = new_part_prod;
  return prev;
}

int mult_large_int(node *multiplicand, node *multiplier, node **product) {
  if (multiplicand == NULL || multiplier == NULL)
    return 0;

  node *prev = NULL, *cur_prod = NULL, *part_prod = NULL;

  int carry, result;
  for (node *ptr_a = multiplier; ptr_a != NULL; ptr_a = ptr_a->next) {
    cur_prod = NULL;
    carry = 0;
    node *ptr_c = NULL;
    for (node *ptr_b = multiplicand; ptr_b != NULL; ptr_b = ptr_b->next) {
      result = ptr_a->value * ptr_b->value + carry;
      carry = result / (MAX + 1);
      result = result % (MAX + 1);
      append_node(&ptr_c, result);
      if (cur_prod == NULL)
        cur_prod = ptr_c;
    }
    if (carry)
      append_node(&ptr_c, carry);
    prev = mult_util_add(part_prod, cur_prod, prev);
    if (*product == NULL)
      *product = prev;

    part_prod = prev->next;
  }
  return 1;
}

void flush_blanks() {
  char c;
  if ((c = getc(stdin)) == ' ' || c == '\n' || c == '\t')
    while ((c = getc(stdin)) == ' ' || c == '\n' || c == '\t')
      ;
  ungetc(c, stdin);
}

bool read_large_int(node **head) {
  int val = 0;
  char c, c_prev = ',';
  bool error = false;
  flush_blanks();
  while (!error && (c = getc(stdin)) != '$') {
    if (c == ',') {
      if (c_prev == ',')
        error = true;
      else {
        add_node(val, head);
        val = 0;
      }
    } else if (c >= '0' && c <= '9') {
      val = val * 10 + c - '0';
      if (val > MAX)
        error = true;
    } else
      error = true;
    c_prev = c;
  }
  if (!error)
    add_node(val, head);
  while (error && (c = getc(stdin)) != '$')
    ;
  return !error;
}

int eval_large_expr() {
  char c;
  flush_blanks();
  node *result = NULL;
  bool error = false;
  read_large_int(&result);
  while (!error) {
    flush_blanks();
    c = getc(stdin);
    if (c == '*') {
      node *oprnd1 = NULL, *oprnd2 = NULL;
      oprnd1 = result;
      if (!read_large_int(&oprnd2))
        error = true;
      result = NULL;
      if (!error)
        mult_large_int(oprnd1, oprnd2, &result);
      dealloc_list(&oprnd1);
      dealloc_list(&oprnd2);
    } else if (c == '+') {
      node *oprnd1 = NULL, *oprnd2 = NULL;
      oprnd1 = result;
      if (!read_large_int(&oprnd2))
        error = true;
      result = NULL;
      if (!error)
        add_large_int(oprnd1, oprnd2, &result);
      dealloc_list(&oprnd1);
      dealloc_list(&oprnd2);
    } else if (c == '=') {
      print_large_int(result);
      break;
    } else {
      printf("Wrong input format\n");
      error = true;
    }
  }
  while (error && (c = getc(stdin)) != '=')
    ;
  dealloc_list(&result);
}
