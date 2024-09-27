
#include <stdio.h>
#include <assert.h>

#undef getchar
int getchar(void)
{
    assert(stdin != NULL);
    return getc(stdin);
}

