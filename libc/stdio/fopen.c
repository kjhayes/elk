
#include "elk-libc-internal/FILE.h"
#include "elk-libc-internal/__sFILE.h"

#include "kanawha/sys-wrappers.h"
#include "kanawha/uapi/file.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#undef fopen
FILE *fopen(
        const char * restrict filename,
        const char * restrict mode)
{
    struct __sFILE *file = malloc(sizeof(struct __sFILE));
    if(file == NULL) {
        return NULL;
    }

    unsigned long access_flags = 0;
    unsigned long mode_flags = 0;

    // TODO: Decode "mode"
    if(strchr(mode, 'r') != NULL) {
        access_flags |= FILE_PERM_READ;
    }
    if(strchr(mode, 'w') != NULL) {
        access_flags |= FILE_PERM_WRITE;
    }

    int res = kanawha_sys_open(
            filename,
            access_flags,
            mode_flags,
            &file->__fd);

    if(res) {
        free(file);
        return NULL;
    }

    return (FILE*)file;
}

