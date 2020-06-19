#include "large_int_header.h"
int main() {
  bool flag = true;
  int choice;
  bool valid;
  node *augend = NULL, *addend = NULL, *sum = NULL;
  node *multiplicand = NULL, *multiplier = NULL, *product = NULL;
  while (flag) {
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      valid = true;
      if (!read_large_int(&augend)) {
        printf("Wrong input format\n");
        valid = false;
      }
      if (!read_large_int(&addend)) {
        printf("Wrong input format\n");
        valid = false;
      }
      if (valid) {
        add_large_int(augend, addend, &sum);
        print_large_int(sum);
      }
      dealloc_list(&augend);
      dealloc_list(&addend);
      dealloc_list(&sum);
      break;
    case 2:
      valid = true;
      if (!read_large_int(&multiplicand)) {
        printf("Wrong input format\n");
        valid = false;
      }
      if (!read_large_int(&multiplier)) {
        printf("Wrong input format\n");
        valid = false;
      }
      if (valid) {
        mult_large_int(multiplicand, multiplier, &product);
        print_large_int(product);
      }
      dealloc_list(&multiplicand);
      dealloc_list(&multiplier);
      dealloc_list(&product);
      break;
    case 3:
      eval_large_expr();
      break;
    default:
      flag = false;
    }
  }
}
