
#include "elk-libc-internal/__sFILE.h"
#include "kanawha/sys-wrappers.h"
#include "kanawha/uapi/file.h"

#include <stdio.h>
#include <string.h>

#undef fputc
int fputc(int i, FILE * restrict stream)
{
    ssize_t res;
    struct __sFILE *file = (struct __sFILE *)stream;

    char c = (char)i;

    res = kanawha_sys_write(
            file->__fd,
            &c,
            sizeof(char));
    if(res != 1) {
        // TODO set FILE error
        return EOF;
    }

    return c;
}

