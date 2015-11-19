
#include <stdio.h>
#include <stdlib.h>
#include <memory/memory.h>

#define LOOP 10
#define ARRAY_LENGTH 10

struct ordinary {
	int a;
	float b;
	double c;
	long d;
	char e;
	void *f;
};

int main(){
	memory_free(memory_alloc(2048));
	int *i = (int*) memory_alloc(sizeof(int));
	int *i2 = (int*) memory_alloc(sizeof(int));
	*i = 2;
	*i2 = 100;
	printf("%d + %d = %d\n", *i, *i2, *i + *i2);
	memory_free(i2);
	memory_free(i);
	i = (int*) memory_alloc(sizeof(int));
	*i = 0;
	unsigned int index = 0;
	for(index = 0; index < LOOP; index++ ){
		int *int_ptr = (int*) memory_alloc(sizeof(int));
		*int_ptr = index;
		*i += *int_ptr;
		memory_free(int_ptr);
	}
	printf("The sum of 0 until %d is %d\n", LOOP, *i);
	memory_free(memory_alloc(2048));
	memory_free(memory_alloc(2048));
	memory_free(memory_alloc(2048 * 5));
	struct ordinary *op =  (struct ordinary*) memory_alloc(sizeof(struct ordinary));
	op->a = 1;
	op->b = 1.0f;
	op->c = 9.99;
	op->d = 999l;
	op->e = 'a';
	op->f = i;
	printf("a = %d, b = %f, c = %f, d = %ld, e = %c, f = %p\n", op->a, op->b, op->c, op->d, op->e, op->f);
	memory_free(op);
	int *ia = (int*) memory_alloc(sizeof(int) * ARRAY_LENGTH);
	for(index = 0; index < ARRAY_LENGTH; ++index){
		ia[index] = index * 2;
	}
	printf("[ ");
	for(index = 0; index < ARRAY_LENGTH; ++index){
		if(index < ARRAY_LENGTH -1)
			printf("%d, ", ia[index]);
		else
			printf("%d", ia[index]);
	}
	printf(" ]\n");
	memory_free(ia);
	exit(EXIT_SUCCESS);
}
