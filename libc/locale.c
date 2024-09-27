
#include <locale.h>
#include <limits.h>
#include <string.h>

const static struct lconv
__ELK_c_locale = {
    .decimal_point = ".",
    .thousands_sep = "",
    .grouping = "",
    .mon_decimal_point = "",
    .mon_thousands_sep = "",
    .positive_sign = "",
    .negative_sign = "",
    .currency_symbol = "",
    .frac_digits = CHAR_MAX,
    .p_cs_precedes = CHAR_MAX,
    .n_cs_precedes = CHAR_MAX,
    .p_sep_by_space = CHAR_MAX,
    .n_sep_by_space = CHAR_MAX,
    .p_sign_posn = CHAR_MAX,
    .n_sign_posn = CHAR_MAX,
    .int_curr_symbol = "",
    .int_frac_digits = CHAR_MAX,
    .int_p_cs_precedes = CHAR_MAX,
    .int_n_cs_precedes = CHAR_MAX,
    .int_p_sep_by_space = CHAR_MAX,
    .int_n_sep_by_space = CHAR_MAX,
    .int_p_sign_posn = CHAR_MAX,
    .int_n_sign_posn = CHAR_MAX,
};

static struct lconv
__ELK_current_locale = __ELK_c_locale;

char *setlocale(int category, const char *locale)
{
    // We only support the "C" locale
    if(strcmp(locale, "") == 0) {
        return "C";
    }
    if(strcmp(locale, "C") == 0) {
        return "C";
    }
    return NULL;
}

struct lconv *localeconv(void)
{
    return &__ELK_current_locale;
}

