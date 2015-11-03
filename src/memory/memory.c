#include <unistd.h>
#include "memory.h"

#define HEADER_SIZE sizeof(struct Header)
struct Header {
	size_t length;
	struct Header *previous;
	struct Header *next;
};

static struct Header *free_list = NULL;

void* memory_alloc(size_t bytes)
{
	if(!free_list){
		free_list = (struct Header*) sbrk(bytes + HEADER_SIZE);
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
		struct Header *new_free_block = header + HEADER_SIZE;
		new_free_block->length = extra - HEADER_SIZE;
		new_free_block->previous = NULL;
		new_free_block->next = free_list;
	} else if(extra < 0){
		//need more memory
		header = (struct Header*) sbrk(bytes + HEADER_SIZE);
	}
	if(header->previous)
		header->previous->next = header->next;
	if(header->next)
		header->next->previous = header->previous;
	if(header->length)
		header->length = header->length - extra;
	if(header == free_list)
		free_list = NULL;
	return header + HEADER_SIZE;
}

void memory_free(void* ptr)
{
	struct Header* memory_block = (struct Header*) (ptr - HEADER_SIZE);
	memory_block->previous = NULL;
	memory_block->next = free_list;
	free_list = ptr;
}
