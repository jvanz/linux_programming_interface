#ifndef _MEMORY_HEADER
#define  _MEMORY_HEADER

/**
 * Allocates a block of memory with bytes of length
 * @returns a pointer to the allocated block of memory. Return NULL in error
 */
void* memory_alloc(size_t bytes);

/**
 * Deallocates the ptr block of memory
 * @returns 0 on success
 */
void memory_free(void* ptr);

#endif
