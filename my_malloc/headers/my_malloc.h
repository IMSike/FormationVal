#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef M_MXFAST
#define M_MXFAST 1
#endif

#ifndef M_NLBLOCKS
#define M_NLBLOCKS 2
#endif

/* Prototypes */
void *mon_malloc(size_t size);
void mon_free(void *ptr);
void *mon_calloc(size_t nmemb, size_t size);
void *mon_realloc(void *ptr, size_t size);
int mon_mallopt(int cmd, int val);

#endif