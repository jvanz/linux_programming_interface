/**
 * This source code contains a simple malloc functions implementation.
 * It was created just for study purposes
 */
#include <unistd.h>
#include "memory.h"
#ifdef DEBUG
#include <stdio.h>
#endif

#define HEADER_SIZE sizeof(Header)
#define MIN_ALLOC HEADER_SIZE * 10

struct header {
	size_t length; //memory block length in bytes
	struct header *next; //next memory block in the free list
};
typedef struct header Header;

#ifdef DEBUG
static void show_debug_info(void);
#endif

static Header *free_list = NULL;
static Header base;

void* memory_alloc(size_t bytes)
{
	Header *block;
	Header *previous = &base;
	if(!free_list){
		//there isn't any memory block available
		Header *block;
		if(bytes <= MIN_ALLOC)
			block = (Header*) sbrk(MIN_ALLOC);
		else
			block = (Header*) sbrk(bytes * MIN_ALLOC);
		block->length = bytes;
		block->next = &base;
		base.length = 0;
		base.next = block;
		free_list = &base;
	}
	//look for a memory block with enough length
	for(block = base.next; block != &base; block = block->next, previous = block){
		if(bytes <= block->length){
			if(bytes == block->length){ //the current memory block has exactly size! \o/
				previous->next = block->next;
				block->next = NULL;
#ifdef DEBUG
				show_debug_info();
#endif
				return block + 1;
			}
			//the block is bigger. Let's split it
			Header *remain_block = block + 1 + bytes;
			remain_block->length = block->length - bytes;
			remain_block->next = block->next;
			previous->next = remain_block;
#ifdef DEBUG
			show_debug_info();
#endif
			return block + 1;
		}
	}
}

void memory_free(void* ptr)
{
	Header *free_block = ((Header*)ptr) - 1;
	Header *block;
	Header *previous = &base;
	for(block = base.next; block != &base; block = block->next){
		//let's find an adjacent memory block
		if((block + 1 + block->length) == free_block){
			block->length += free_block->length;
		} else if((block - block->length) == free_block){
			free_block->length += block->length;
			free_block->next = block->next;
			previous->next = free_block;
		}
	}
}

#ifdef DEBUG
static void show_debug_info(){}
#endif
