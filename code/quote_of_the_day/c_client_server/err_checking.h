#ifndef ERR_CHECKING_H
#define ERR_CHECKING_H
// takes in error message to print if failed and received and expected return
// values as well as comparison to perform on the two
void func_err_check(const char *ERR_MSG, int ret_val,
                    int (*comparison)(int, int), int expected_val);

#endif
#ifndef ERR_CHECKING_SRC
#define ERR_CHECKING_SRC
#include <stdio.h>
#include <stdlib.h>

void func_err_check(const char *ERR_MSG, int ret_val,
                    int (*comparison)(int, int), int expected_val) {
  if (comparison(ret_val, expected_val)) {
    fprintf(stderr, "[ERROR] %s\n", ERR_MSG);
    if (DEBUG_STATEMENTS)
      printf("[DEBUG] Error code: %d\n", ret_val);
    exit(EXIT_FAILURE);
  }
}

#endif
