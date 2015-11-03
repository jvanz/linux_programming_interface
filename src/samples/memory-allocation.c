
#include <stdio.h>
#include <mcheck.h>
#include <memory/memory.h>

int main(int argc, char **argv){
	mtrace();
	void *memory = memory_alloc(1024);
	memory_free(memory);
	muntrace();
}
