#ifndef __FFP_UTILS_H__
/*
    Utilities
*/
#define __FFP_UTILS_H__

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Checks if the character is the null terminator
 *
 * @param c Character to check 
 *
 * @returns 1 if true, 0 if false
*/
int is_null(char c);

/**
 * @brief Trims a string off of a specific
 * delimeter either trailling or leading
 *
 * @param str   String to trim
 * @param delim Delimter to remove
 * 
 * @note The constructed stirng is at most 512 characters.
 *
 * @returns Trimmed string
*/
char* trim_outer_string(const char* str, char delim);

#endif /* __FFP_UTILS_H__ */