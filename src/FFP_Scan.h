#ifndef __FFP_SCAN_H__
/*
    Primary file scanning functions
*/
#define __FFP_SCAN_H__

#include "FFP_File.h"
#include "FFP_Utils.h"

static unsigned int FFP_MAX_LINE_LENGTH = 1024; /* Max line length */

/**
 * @brief Reads a value of name value_string
 * and returns its value as a string
 * 
 * @param value_string Name of string/field to get the value from
 * 
 * @returns The value of the corresponding string/field inside the ffp file, as a string
 */
char* ffp_read_value(const char* value_string);

#endif /* __FFP_SCAN_H__ */