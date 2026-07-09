#include "FFP_Utils.h"

int is_null(char c) {
    return c == '\0';
}

char* trim_outer_string(const char* str) {
    if (!str)
        return NULL;

    char* construct = malloc(512);
    if (!construct)
        return NULL;

    size_t len = 0;
    int started = 0;

    /* Copy, skipping leading whitespace */
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!started && isspace((unsigned char)str[i]))
            continue;

        started = 1;
        construct[len++] = str[i];
    }

    if (!started) {
        construct[0] = '\0';
        return construct;
    }

    /* Remove trailing whitespace */
    while (len > 0 && isspace((unsigned char)construct[len - 1]))
        len--;

    construct[len] = '\0';

    return construct;
}

int str_compare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2)
            return 0;
        str1++;
        str2++;
    }

    return *str1 == *str2;
}
