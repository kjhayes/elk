
#include "kanawha/sys-wrappers.h"
#include "kanawha/uapi/mmap.h"

#include <stddef.h>

void *malloc(size_t size)
{
    int res;
    void *addr = NULL;

    // Align up to the nearest page
    size_t alloc_size = size + 16;
    if(alloc_size & 0xFFF) {
        alloc_size &= ~0xFFF;
        alloc_size += 0x1000;
    }

    res = kanawha_sys_mmap(
            0, // No file
            0, // No file offset
            &addr,
            alloc_size,
            MMAP_PROT_READ|MMAP_PROT_WRITE|MMAP_ANON);
    if(res) {
        return NULL;
    }

    uint64_t *sizes_ptr = (uint64_t*)addr;
    sizes_ptr[0] = size;

    return (addr + 16);
}

