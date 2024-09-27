
#include "kanawha/sys-wrappers.h"
#include "kanawha/uapi/mmap.h"
#include "kanawha/uapi/file.h"

#include <stddef.h>
#include <stdlib.h>

void *calloc(size_t nmemb, size_t size)
{
    size_t total_size = nmemb * size;
    // mmap will zero the region for us
    return malloc(total_size);
}

