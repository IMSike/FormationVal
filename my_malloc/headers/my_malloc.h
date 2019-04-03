#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stddef.h>

/* Prototypes */
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif