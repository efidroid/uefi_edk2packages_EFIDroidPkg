#ifndef _STDLIB_H
#define _STDLIB_H

#include <stdint.h>
#include <Library/MallocLib.h>

int atoi(const char *num);

void *bsearch(
    const void *key, const void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *)
);

#endif
