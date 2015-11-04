#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char etext, edata, end;

void print_segments();

int main()
{
	printf("First address past:\n");
	printf("    program text (etext)      %10p\n", &etext);
	printf("    initialized data (edata)  %10p\n", &edata);
	printf("    uninitialized data (end, BBS)  %10p\n", &end);
	printf("    sbrk(0)  %10p\n", sbrk(0));
	void* memall = malloc(131072);
	printf("After memory allocation :\n");
	printf("    sbrk(0)  %10p\n", sbrk(0));
	void* memall2 = malloc(131072);
	printf("After memory allocation :\n");
	printf("    sbrk(0)  %10p\n", sbrk(0));
	free(memall2);
	printf("After freed memory allocation :\n");
	printf("    sbrk(0)  %10p\n", sbrk(0));
	free(memall);
	printf("After freed memory allocation :\n");
	printf("    sbrk(0)  %10p\n", sbrk(0));
	exit(EXIT_SUCCESS);
}
