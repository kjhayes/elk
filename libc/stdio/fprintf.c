
#include <stdarg.h>
#include <stdio.h>

int
fprintf (FILE * restrict stream, const char *format, ...)
{
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stream, format, arg);
   va_end (arg);

   return done;
}

