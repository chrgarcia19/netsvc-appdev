#ifndef FIBONACCI_H
#define FIBONACCI_H

unsigned long fibonacci(unsigned long index);

#endif

#ifndef FIBONACCI_SRC
#define FIBONACCI_SRC
#include <stdlib.h>
// returns the fibonacci number at `index`
// index must be greater than 0
unsigned long fibonacci(unsigned long index) {
  unsigned long fib1 = 1;
  unsigned long fib2 = 1;
  unsigned long fib3 = fib1 + fib2;

  if (index == 0){
    printf("[ERROR] Fibonacci index must be greater than 0\n");
    exit(1);
  }
  else if (index == 1 || index == 2) {
    return (index == 1 ? fib1 : fib2);
  }

  for (unsigned long i = 2; i < index; i++) {
    fib1 = fib2;
    fib2 = fib3;
    fib3 = fib1 + fib2;
  }

  return fib3;
}

#endif
