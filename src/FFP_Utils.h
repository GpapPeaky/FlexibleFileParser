#ifndef __FFP_UTILS_H__
/*
    Utilities
*/
#define __FFP_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Checks if the character is the null terminator
 *
 * @param c Character to check 
 *
 * @returns 1 if true, 0 if false
*/
int is_null(char c);

/**
 * @brief Trims a string off of whitespaces and escape sequences
 *
 * @param str String to trim
 * 
 * @note The constructed stirng is at most 512 characters.
 *
 * @returns Trimmed string
*/
char* trim_outer_string(const char* str);

/**
 * @brief Compares 2 strings with eachother
 * 
 * @param str1 String 1
 * @param str2 String 2
 * 
 * @returns 1 if true, 0 if false
 */
int str_compare(const char* str1, const char* str2);

#endif /* __FFP_UTILS_H__ */