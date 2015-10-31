#include <unistd.h>
#include "memory.h"

#define BRK_INCREMENT = 1024;

struct Header {
	size_t length;
	struct Header *previous;
	struct Header *next;
};

static struct Header *free_list = NULL;

void* memory_alloc(size_t bytes)
{
	if(!free_list){
		free_list = (struct Header*) sbrk(bytes + sizeof(struct Header));
		free_list->length = bytes;
		free_list->previous = NULL;
		free_list->next = NULL;
	}
	struct Header *header = free_list;
	while(header && header->length < bytes)
		//look for a memory block with enough memory
		header = header->next;
	int extra = 0;
	if((extra = header->length - bytes) > 0){
		//memory block is bigger than needed
		struct Header *new_free_block = header + sizeof(struct Header) + 1;
		new_free_block->length = extra - sizeof(struct Header);
		new_free_block->previous = NULL;
		new_free_block->next = free_list;
	} else if(extra < 0){
		//need more memory
		header = (struct Header*) sbrk(bytes + sizeof(struct Header));
	}
	header->previous->next = header->next;
	header->next->previous = header->previous;
	header->length = header->length - extra;
	return header + sizeof(struct Header);
}

void memory_free(void* ptr)
{
	struct Header* memory_block = (struct Header*) ptr;
	memory_block->previous = NULL;
	memory_block->next = free_list;
}
