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
#define NUNITS(bytes) bytes / HEADER_SIZE

struct header {
	unsigned int size; //memory block size in bytes
	struct header *next; //next memory block in the free list
};
typedef struct header Header;

#ifdef DEBUG
static void show_debug_info(void);
#endif
static Header* increase_heap(unsigned int);

static Header *free_list = NULL;
static Header base;

void* memory_alloc(size_t bytes)
{
	Header *block;
	Header *previous = &base;
	unsigned int nunits = NUNITS(bytes);
	if(!nunits)
		nunits = 1;
	if(!free_list){
		//first call
		Header *block = increase_heap(nunits);
		block->next = &base;
		base.size = 0;
		base.next = block;
		free_list = &base;
	}
	//look for a memory block with enough size
	for(block = base.next; block != &base; previous = block, block = block->next ){
		if(nunits <= block->size){
			if(nunits == block->size){ //the current memory block has exactly size! \o/
				previous->next = block->next;
				block->next = NULL;
#ifdef DEBUG
				show_debug_info();
#endif
				return block + 1;
			}
			//the block is bigger. Let's split it
			Header *remain_block = block + 1 + nunits;
			remain_block->size = block->size - nunits;
			remain_block->next = block->next;
			previous->next = remain_block;
			block->size = nunits;
#ifdef DEBUG
			show_debug_info();
#endif
			return block + 1;
		}
	}
	//need more memory
	block = increase_heap(nunits);
	return block + 1;
}

void memory_free(void* ptr)
{
	Header *fblock = ((Header*)ptr) - 1;
	Header *block;
	Header *previous = &base;
	for(block = base.next; block != &base; previous = block, block = block->next){
		//let's find an adjacent memory block
		if((block + 1 + block->size) == fblock){
			block->size += fblock->size;
		} else if((block - block->size -1) == fblock){
			fblock->size += block->size;
			fblock->next = block->next;
			previous->next = fblock;
		}
	}
	previous->next = fblock;
	fblock->next = &base;
	ptr = NULL;
#ifdef DEBUG
	show_debug_info();
#endif
}

Header* increase_heap(unsigned int units)
{
#define MIN_ALLOC HEADER_SIZE * 2
	Header *block;
	if(units <= 1)
		block = (Header*) sbrk(MIN_ALLOC);
	else
		block = (Header*) sbrk( 1 + units * HEADER_SIZE);
	block->size = units;
	block->next = NULL;
	return block;
}

#ifdef DEBUG
static void show_debug_info()
{
	Header *block;
	for(block = base.next; block != &base; block = block->next){
		printf("-> %zu ", block->size);
	}
	printf("\n");
}
#endif
