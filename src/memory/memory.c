/**
 * This source code contains a simple malloc functions implementation.
 * It was created just for study purposes
 */
#include <unistd.h>
#include "memory.h"
#ifdef DEBUG
#include <stdio.h>
#endif

#define HEADER_SIZE sizeof(struct Header)

struct Header {
	size_t length; //memory block length in bytes
	struct Header *previous; //previous memory block in the free list
	struct Header *next; //next memory block in the free list
};
#ifdef DEBUG
void get_free_memory_blocks(void);
#endif

static struct Header *free_list = NULL;

void* memory_alloc(size_t bytes)
{
	if(!free_list){
		//there isn't any memory block available
		free_list = (struct Header*) sbrk(bytes + HEADER_SIZE);
		free_list->length = bytes;
		free_list->previous = NULL;
		free_list->next = NULL;
	}
	struct Header *header = free_list;
	//look for a memory block with enough length
	while(header && (header->length < bytes || (header->length > bytes && (header->length - bytes < HEADER_SIZE + 1))))
		header = header->next;
	int extra = 0;
	if(header && (extra = header->length - bytes) > 0){
		//memory block is bigger than needed
		struct Header *new_free_block = (struct Header*) (((char*)header) + HEADER_SIZE + bytes) ;
		new_free_block->length = extra - HEADER_SIZE;
		new_free_block->next = header->next;
		header->next = new_free_block;
		new_free_block->previous = header;
	} else if(extra < 0 || !header){
		//need more memory
		header = (struct Header*) sbrk(bytes + HEADER_SIZE);
		header->length = bytes;
		free_list->previous = NULL;
		free_list->next = NULL;
	}
	if(header->previous)
		header->previous->next = header->next;
	if(header->next)
		header->next->previous = header->previous;
	header->length = header->length - extra;
	if(header == free_list) {
		free_list = header->next;
		if(free_list)
			free_list->previous = NULL;

	}
#ifdef DEBUG
get_free_memory_blocks();
#endif
	return (void*) (((char*)header) + HEADER_SIZE);
}

void memory_free(void* ptr)
{
	struct Header* memory_block = (struct Header*) (((char*)ptr) - HEADER_SIZE);
	memory_block->previous = NULL;
	memory_block->next = free_list;
	if(memory_block->next)
		memory_block->next->previous = memory_block;
	free_list = memory_block;
	ptr = NULL;
#ifdef DEBUG
get_free_memory_blocks();
#endif

}

#ifdef DEBUG
void get_free_memory_blocks(){
	struct Header *header = free_list;
	int total = 0;
	//look for a memory block with enough memory
	while(header){
		printf("-> %zu bytes ", header->length);
		header = header->next;
		total += 1;
	}
	printf("\nTotal memory blocks: %d\n", total);
}
#endif
