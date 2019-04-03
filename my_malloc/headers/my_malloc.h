#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Prototypes */
void *mon_malloc(size_t size);
void mon_free(void *ptr);
void *mon_calloc(size_t nmemb, size_t size);
void *mon_realloc(void *ptr, size_t size);

#endif