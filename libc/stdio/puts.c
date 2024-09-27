
#include <stdio.h>

#undef puts
int puts(const char *s)
{
    return fputs(s, stdout);
}

