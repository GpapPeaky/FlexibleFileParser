#ifndef __FFP_FILE_H__
/*
    Primary file manipulation
*/
#define __FFP_FILE_H__

#include <stdio.h>

extern FILE* ffp_file; /* File global to read from, if requiring to read from another file, we need to rebind it */

/**
 * @brief Bind a file to the ffp_file global
 * 
 * @param file File to open
 */
void ffp_bind_file(FILE* file);

/**
 * @brief Nullifies the ffp_file global 
 */
void ffp_unbind_file(void);

/**
 * @brief Checks if any file is bound to the ffp_file global
 * 
 * @returns 1 if true, 0 if false
 */
int ffp_file_bound(void);

#endif /* __FFP_FILE_H__ */