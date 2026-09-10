#ifndef __FFP_H__
#define __FFP_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Public */

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

/* Implementation */

#ifdef __FFP_IMPL__

#pragma region private utilities

static int is_null(char c)
{
    return c == '\0';
}

static int FFP_MAX_LINE_LENGTH = 512; /* Max file length for required memory allocations */

static char* trim_outer_string(const char* str)
{
    if (!str)
        return NULL;

    char* construct = malloc(FFP_MAX_LINE_LENGTH);
    if (!construct)
        return NULL;

    size_t len = 0;
    int started = 0;

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

    while (len > 0 && isspace((unsigned char)construct[len - 1]))
        len--;

    construct[len] = '\0';

    return construct;
}

static int str_compare(const char* str1, const char* str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2)
            return 0;

        str1++;
        str2++;
    }

    return *str1 == *str2;
}

#pragma endregion private utilities

#pragma region file

FILE* ffp_file = NULL;

void ffp_bind_file(FILE* file) {
    ffp_file = file;

    if (!ffp_file_bound()) {
        fprintf(stderr, "Failed to bind file\n");
        return;
    }

    char checksum[4];

    fscanf(ffp_file, "%3s", checksum);

    if (!str_compare(checksum, "FFP")) {
        fprintf(
            stderr,
            "Invalid file format, file needs to be specified as FFP at line 1\n"
        );

        ffp_unbind_file();
        return;
    }
}

void ffp_unbind_file(void) {
    ffp_file = NULL;
}

int ffp_file_bound(void) {
    return ffp_file != NULL;
}

#pragma endregion file

#pragma region scan

static int ffp_validate_line_format(const char* line) {
    // Format:
    //
    // field=value   -> integer
    // field="value" -> string

    char* trimmed = trim_outer_string(line);

    // printf("LINE %s\n", line);
    // printf("TRIMMED %s\n", trimmed);

    char* compressed = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);

    size_t idx = 0;
    size_t whitespace_count = 0;
    while (!is_null(trimmed[idx])) {
        if (trimmed[idx] == ' ') {
            whitespace_count++;
            
            idx++;

            continue; // Skip whitespace
        }
        
        compressed[idx - whitespace_count] = trimmed[idx];

        idx++;
    }

    idx = 0;

    // printf("COMPRESSED %s\n", compressed);

    typedef enum {
        FIELD = 0,
        OP,
        VAL
    } line_format;

    // Save tokens here, validate later
    char* tokens[3];
    for (int j = 0 ; j < 3 ; j++) {
        tokens[j] = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);
        // tokens[j] = NULL;
    }

    int opFound = 0;
    int opFoundIdx = 0;
    while (!is_null(compressed[idx])) {
        if (compressed[idx] != '=' && !opFound) {
            tokens[FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[VAL][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[OP][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    // printf("%s\n", tokens[FIELD]);
    // printf("%s\n", tokens[OP]);
    // printf("%s\n", tokens[VAL]);

    return tokens[FIELD] && tokens[OP] && tokens[VAL];
} 

static char* ffp_get_field_from_line(const char* line) {
    // Format:
    //
    // field=value   -> integer
    // field="value" -> string

    char* trimmed = trim_outer_string(line);

    // printf("LINE %s\n", line);
    // printf("TRIMMED %s\n", trimmed);

    char* compressed = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);

    size_t idx = 0;
    size_t whitespace_count = 0;
    while (!is_null(trimmed[idx])) {
        if (trimmed[idx] == ' ') {
            whitespace_count++;
            
            idx++;

            continue; // Skip whitespace
        }
        
        compressed[idx - whitespace_count] = trimmed[idx];

        idx++;
    }

    idx = 0;

    // printf("COMPRESSED %s\n", compressed);

    typedef enum {
        FIELD = 0,
        OP,
        VAL
    } line_format;

    // Save tokens here, validate later
    char* tokens[3];
    for (int j = 0 ; j < 3 ; j++) {
        tokens[j] = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);
        // tokens[j] = NULL;
    }

    int opFound = 0;
    int opFoundIdx = 0;
    while (!is_null(compressed[idx])) {
        if (compressed[idx] != '=' && !opFound) {
            tokens[FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[VAL][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[OP][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    return tokens[FIELD];
}

static char* ffp_get_value_from_line(const char* line) {
    // Format:
    //
    // field=value   -> integer
    // field="value" -> string

    char* trimmed = trim_outer_string(line);

    // printf("LINE %s\n", line);
    // printf("TRIMMED %s\n", trimmed);

    char* compressed = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);

    size_t idx = 0;
    size_t whitespace_count = 0;
    while (!is_null(trimmed[idx])) {
        if (trimmed[idx] == ' ') {
            whitespace_count++;
            
            idx++;

            continue; // Skip whitespace
        }
        
        compressed[idx - whitespace_count] = trimmed[idx];

        idx++;
    }

    idx = 0;

    // printf("COMPRESSED %s\n", compressed);

    typedef enum {
        FIELD = 0,
        OP,
        VAL
    } line_format;

    // Save tokens here, validate later
    char* tokens[3];
    for (int j = 0 ; j < 3 ; j++) {
        tokens[j] = malloc(sizeof(char) * FFP_MAX_LINE_LENGTH);
        // tokens[j] = NULL;
    }

    int opFound = 0;
    int opFoundIdx = 0;
    while (!is_null(compressed[idx])) {
        if (compressed[idx] != '=' && !opFound) {
            tokens[FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[VAL][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[OP][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    return tokens[VAL];
}

void* ffp_read_value(const char* value_string) {
    if (!ffp_file_bound()) {
        fprintf(stderr, "File not bound to read from");
        return NULL;
    }

    rewind(ffp_file);

    // For now we will brute force it by reading the entire file
    // and checking for the valuw_string 

    if (!ffp_file_bound()) {
        fprintf(stderr,"File not bound to read from after rewind");
        return NULL;
    }

    char line[FFP_MAX_LINE_LENGTH];
    long long unsigned int lineNum = 1; 

    while (fgets(line, sizeof(line), ffp_file) != NULL) {
        if (!line) continue;

        int valid = ffp_validate_line_format(line);

        if (!valid) {
            fprintf(stderr,"Invalid line format at line %lld\n", lineNum);
            continue;
        }

        char* field = ffp_get_field_from_line(line);

        if (str_compare(value_string, field)) {
            // printf("Field \'%s\', found\n", field);

            return ffp_get_value_from_line(line);
        }

        lineNum++;
    }
    
    return NULL; // Not found
}

#pragma endregion scan

#endif /* __FFP_IMPL__ */

#endif /* __FFP_H__ */