
#include "kanawha/sys-wrappers.h"
#include "elk-libc-internal/__sFILE.h"

#include <stdio.h>

int
fseek(
        FILE *stream,
        long int offset,
        int whence)
{
    int res;

    res = kanawha_sys_seek(
            stream->__fd,
            offset,
            whence);
    if(res) {
        return res;
    }

    return 0;
}

