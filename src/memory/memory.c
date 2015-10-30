#include <unistd.h>
#include "memory.h"

#define BRK_INCREMENT = 1024;

union Header {
	size_t length;
	struct {
		size_t length;
		union Header *previous;
		union Header *next;
	} free;
};

union Header *free_list = NULL;

void* vmalloc(size_t bytes)
{
	union Header *header = NULL;
	if(!free_list){
		free_list = (union Header*) sbrk(bytes + sizeof(union Header));
		free_list->free.length = bytes;
		free_list->free.previous = NULL;
		free_list->free.next = NULL;
	}
	while(free_list && free_list->free.length < bytes)
		free_list = free_list->free.next;
	if(!free_list){
		//need more memory
		header = (union Header*) sbrk(bytes + sizeof(union Header));
	} else {
		header = free_list;
		free_list->free.previous->free.next = free_list->free.next;
		free_list->free.next->free.previous = free_list->free.previous;
		int extra = 0;
		if((extra = header->free.length - bytes) > 0){
			union Header *new_free_block = header + sizeof(union Header) + 1;
			new_free_block->free.length = extra - sizeof(union Header);
			new_free_block->free.previous = NULL;
			new_free_block->free.next = free_list;
		}
		header->length = header->free.length - extra;
	}
	return header + sizeof(union Header);
}

int vfree(void* ptr)
{
	return 0;
}
