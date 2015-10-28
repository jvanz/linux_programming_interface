#ifndef _MEMALLOC_HEADER
#define  _MEMALLOC_HEADER

/**
 * Allocates a block of memory with bytes of length
 * @returns a pointer to the allocated block of memory. Return NULL in error
 */
void* valloc(size_t bytes);

/**
 * Deallocates the ptr block of memory
 * @returns 0 on success
 */
int vfree(void* ptr);

#endif
