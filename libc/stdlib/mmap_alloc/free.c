
#include "kanawha/sys-wrappers.h"
#include "kanawha/uapi/mmap.h"
#include "kanawha/uapi/file.h"

#include <stddef.h>
#include <assert.h>

void free(void *ptr)
{
    int res;
    void *base_ptr = (void*)((uintptr_t)ptr & ~0xFFF);

    res = kanawha_sys_munmap(ptr);
    assert(res == 0);
}

