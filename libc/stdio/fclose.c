
#include "elk-libc-internal/FILE.h"
#include "elk-libc-internal/__sFILE.h"

#include "kanawha/sys-wrappers.h"

#include <stdlib.h>
#include <stdio.h>

int fclose(FILE *stream)
{
    struct __sFILE *file = (struct __sFILE*)stream;

    int res = kanawha_sys_close(
            file->__fd);
    if(res) {
        return EOF;
    }

    free(file);

    return 0;
}

