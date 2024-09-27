
#include <stdio.h>
#include <assert.h>

#undef putchar
int putchar(int c)
{
    assert(stdin != NULL);
    return putc(c, stdout);
}

