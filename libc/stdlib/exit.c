
#include <stdlib.h>
#include <kanawha/sys-wrappers.h>

void
exit(int status)
{
    kanawha_sys_exit(status);
}

