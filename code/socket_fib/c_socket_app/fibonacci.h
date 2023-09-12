#ifndef FIBONACCI_H
#define FIBONACCI_H

unsigned long fibonacci(unsigned long index);

#endif

#ifndef FIBONACCI_SRC
#define FIBONACCI_SRC
#include <stdlib.h>
// returns the fibonacci number at `index`
// index must be greater than or equal to 0
unsigned long fibonacci(unsigned long index){
	unsigned long fib0 = 1;
	unsigned long fib1 = 1;
	unsigned long fib2 = fib0 + fib1;

	if(index == 0 || index == 1){
		return (index == 0 ? fib0 : fib1);
	}

	for(unsigned long i = 2; i < index; i++){
		fib0 = fib1;
		fib1 = fib2;
		fib2 = fib0 + fib1;
	}

	return fib2;
}

#endif
