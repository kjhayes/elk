
#include "kanawha/sys-wrappers.h"
#include <kanawha/uapi/mmap.h>
#include <kanawha/uapi/environ.h>
#include <kanawha/uapi/errno.h>

static size_t
strlen(char *str)
{
    size_t len = 0;
    while(*str) {
        str++;
        len++;
    }
    return len;
}

#define ARGV_ENV_KEY "ARGV"
#define ARGV_REGION_SIZE 0x1000

void
__elk_crt__get_argv(
        int *argc_out,
        const char ***argv_out)
{
    int res;
    void *region_base;
    res = kanawha_sys_mmap(
            0,
            0,
            &region_base,
            ARGV_REGION_SIZE,
            MMAP_PROT_READ|MMAP_PROT_WRITE|MMAP_ANON);
    if(res) {
        kanawha_sys_exit(res);
    }

    res = kanawha_sys_environ(
            ARGV_ENV_KEY,
            region_base,
            ARGV_REGION_SIZE,
            ENV_GET);
    if(res == -ENXIO) {
        kanawha_sys_munmap((void*)region_base);
        *argc_out = 0;
        *argv_out = NULL;
        return;
    }
    else if(res) {
        kanawha_sys_exit(res);
    }

    char *end = region_base + ARGV_REGION_SIZE;
    *(end-1) = '\0';

    size_t argv_len = strlen(region_base) + 1;

    size_t room_left = ARGV_REGION_SIZE - argv_len;
    if(room_left >= ARGV_REGION_SIZE) {
        // Underflow
        kanawha_sys_exit(1);
    }

    const char **argv_ptr = (const char **)(end - room_left);
    size_t argc_max = room_left / sizeof(const char*);

    int argc = 0;

    for(size_t i = 0; i < argv_len; i++) {
        char c = ((char*)region_base)[i];
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            ((char*)region_base)[i] = '\0';
        }
    }

    char *iter = region_base;
    size_t len_left = argv_len;
    while((uintptr_t)iter < (uintptr_t)argv_ptr && len_left > 0) {
        char *cur = iter;
        size_t cur_len = strlen(cur);
        if(cur_len > 0) {
            if(argc < argc_max) {
                argv_ptr[argc] = cur;
                argc++;
            } else {
                // Too many or too long argument(s)
                kanawha_sys_exit(1);
            }
        }
        iter += cur_len + 1;
    }

    *argc_out = argc;
    *argv_out = argv_ptr;

    return;
}

