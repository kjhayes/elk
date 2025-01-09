
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

static int
doprnt_escape_spec(
        int(*putchar)(int c, void *state),
        void *state,
        const char *escape_spec,
        va_list args,
        size_t *speclen_out)
{
    char conv; // conversion specifier
    size_t speclen = 1; // total length of the escape_spec
    const char *iter = escape_spec;

    int found_conv = 0;
    do {
        if(*iter == '\0') {
            return -1;
        }
        switch(*iter) {
            case 'd':
            case 'i':
            case 'o':
            case 'u':
            case 'x':
            case 'X':
            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            case 'a':
            case 'A':
            case 'c':
            case 's':
            case 'p':
            case 'n':
            case '%':
                conv = *iter;
                found_conv = 1;
                break;
            default:
                speclen++;
                iter++;
                break;
        }
    } while(!found_conv);

    *speclen_out = speclen;

    int printed = 0;
#define PUTCHAR(__c) \
    do {\
        int res = (*putchar)(__c, state);\
        if(res < 0) { \
            return res; \
        } \
        printed++;\
    } while(0)

    const char *str;
    void *ptr;

    switch(conv) {
        case '%':
            PUTCHAR('%');
            break;
        case 's':
            str = va_arg(args, const char *);
            while(*str) {
                PUTCHAR(*str);
                str++;
            }
            break;
        case 'p':
            ptr = va_arg(args, void *);
            PUTCHAR('0');
            PUTCHAR('x');
            for(int i = 0; i < sizeof(ptr)*2; i++) {
                uint8_t nibble = ((uintptr_t)ptr >> ((((sizeof(ptr)*2)-1)-i) * 4)) & 0xF;
                if(nibble < 10) {
                    PUTCHAR('0' + nibble);
                } else {
                    PUTCHAR('A' + (nibble - 10));
                }
            }
    }

    return printed;
}

int
doprnt(
        int(*putchar)(int c, void *state),
        void *state,
        const char *fmt,
        va_list args)
{
#undef PUTCHAR
#define PUTCHAR(__c) (*putchar)(__c, state)

    int printed = 0;
    const char *iter = fmt;
    while(*iter) {
        if(*iter == '%') {
            int escape_printed;
            
            // Length of fmt specifier (not including the %)
            size_t speclen;

            iter++;

            escape_printed = doprnt_escape_spec(putchar, state, iter, args, &speclen);
            if(escape_printed < 0) {
                return escape_printed;
            }
            printed += escape_printed;
            iter += speclen;

        } else {
            int res = PUTCHAR(*iter);
            if(res < 0) {
                return res;
            }
            iter++;
        }
    }

    return printed;

#undef PUTCHAR
}

