
#include <unistd.h>
#include "memalloc.h"

static void* free_list = NULL;

void* valloc(size_t bytes)
{
	if( !free_list ){
		//TODO init free list
	}
	//find a memory block using first fit
	void *aux = free_list;
	while((int)aux < bytes ){
		aux += (sizeof(int)*2);
	}
	//after find the memory block, remote it from free_list and return it to the caller
	return aux;
}

int vfree(void* ptr)
{
	//TODO
	return -1;
}
