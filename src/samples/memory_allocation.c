
#include <stdio.h>
#include <stdlib.h>
#include <memory/memory.h>

int main(){
	int *i = (int*) memory_alloc(sizeof(int));
	int *i2 = (int*) memory_alloc(sizeof(int));
	*i = 2;
	*i2 = 100;
	printf("%d + %d = %d\n", *i, *i2, *i + *i2);
	memory_free(i);
	memory_free(i2);
	i = (int*) memory_alloc(sizeof(int));
	*i = 0;
	int index = 0;
	for(index = 0; index < 100; index++ ){
		int *int_ptr = (int*) memory_alloc(sizeof(int));
		*int_ptr = index;
		*i += *int_ptr;
		memory_free(int_ptr);
	}
	printf("The sum of 0 until 100 is %d", *i);
	exit(EXIT_SUCCESS);
}
