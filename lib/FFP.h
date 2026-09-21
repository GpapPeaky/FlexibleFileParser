#ifndef FFP_INCLUDED /* Flexible file parser */
#define FFP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Public */

#pragma region public api

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

/**
 * @brief Opens a file of specific name and binds it to the 
 * ffp_file global
 * 
 * @param fname Name of file to bind
 */
void ffp_open(const char* fname);

/**
 * @brief Read a value string from the bound file
 * 
 * @param value_string String of the value name
 * 
 * @returns The scanned string as a char*
 */
char* ffp_read_string(const char* value_string);

/**
 * @brief Read a value int from the bound file
 * 
 * @param value_string String of the value name
 * 
 * @returns The scanned string as an int
 */
int ffp_read_int(const char* value_string);

/**
 * @brief Read a value float from the bound file
 * 
 * @param value_string String of the value name
 * 
 * @returns The scanned string as a float
 */
float ffp_read_float(const char* value_string);

#pragma endregion public api

/* Implementation */
#pragma region impl

#ifdef FFP_IMPL

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

#pragma region private utilities

/* Line format enumerators for validation */
enum {
    FFP_FORMAT_VALIDATOR_FIELD = 0,                 /* Field name */
    FFP_FORMAT_VALIDATOR_OPERATOR,                  /* Operator */
    FFP_FORMAT_VALIDATOR_VALUE                      /* Value of field */
};

static int is_null(char c) {
    return c == '\0';
}

static int ascii_to_int(const char* number_string) {
    int number = 0;
    int neg = number_string[0] == '-';
    int i = neg ? 1 : 0;
    while (number_string[i] >= '0' && number_string[i] <= '9') {
      number *= 10;                     // multiply number by 10
      number += number_string[i] - '0'; // convet ASCII '0'..'9' to digit 0..9 and add it to number           
      i++;                              // step one digit forward
    }

    if(neg) number *= -1;
    
    return number;
}

static float ascii_to_float(const char* number_string) {
    float result = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    int sign = 1;
    int decimal = 0;

    if (*number_string == '-') {
        sign = -1;
        number_string++;
    } else if (*number_string == '+') {
        number_string++;
    }

    while (*number_string != '\0') {
        if (*number_string == '.') {
            decimal = 1;
            number_string++;
            continue;
        }

        if (*number_string >= '0' && *number_string <= '9') {
            if (!decimal) {
                result = result * 10.0f + (*number_string - '0');
            } else {
                fraction = fraction * 10.0f + (*number_string - '0');
                divisor *= 10.0f;
            }
        } else {
            break; // Stop on invalid character
        }

        number_string++;
    }

    return sign * (result + (fraction / divisor));
}

static int FFP_MAX_LINE_LENGTH = 512; /* Max file length for required memory allocations */

static char* trim_outer_string(const char* str) {
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

static int str_compare(const char* str1, const char* str2) {
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
        fprintf(stderr, "[FFP] Failed to bind file\n");
        return;
    }

    char checksum[4];

    fscanf(ffp_file, "%3s", checksum);

    if (!str_compare(checksum, "FFP")) {
        fprintf(
            stderr,
            "[FFP] Invalid file format, file needs to be specified as FFP at line 1\n"
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

void ffp_open(const char* fname) {
    FILE* f = fopen(fname, "r");

    if(!f) fprintf(stderr, "[FFP] Failed to load file with name %s\n", fname);
    
    ffp_bind_file(f);

    if(!ffp_file_bound()) fprintf(stderr, "[FFP] Failed to bind file with name %s\n", fname);
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
            tokens[FFP_FORMAT_VALIDATOR_FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[FFP_FORMAT_VALIDATOR_VALUE][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[FFP_FORMAT_VALIDATOR_OPERATOR][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    // printf("%s\n", tokens[FFP_FORMAT_VALIDATOR_FIELD]);
    // printf("%s\n", tokens[FFP_FORMAT_VALIDATOR_OPERATOR]);
    // printf("%s\n", tokens[FFP_FORMAT_VALIDATOR_VALUE]);

    return tokens[FFP_FORMAT_VALIDATOR_FIELD] && tokens[FFP_FORMAT_VALIDATOR_OPERATOR] && tokens[FFP_FORMAT_VALIDATOR_VALUE];
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
            tokens[FFP_FORMAT_VALIDATOR_FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[FFP_FORMAT_VALIDATOR_VALUE][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[FFP_FORMAT_VALIDATOR_OPERATOR][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    return tokens[FFP_FORMAT_VALIDATOR_FIELD];
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
            tokens[FFP_FORMAT_VALIDATOR_FIELD][idx] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] != '=' && opFound) {
            tokens[FFP_FORMAT_VALIDATOR_VALUE][idx - opFoundIdx - 1] = compressed[idx];

            idx++;

            continue;
        } else if (compressed[idx] == '=') {
            opFound = 1;
            opFoundIdx = idx++;

            tokens[FFP_FORMAT_VALIDATOR_OPERATOR][0] = '=';

            continue;
        } else {
            return 0;
        }
    }

    return tokens[FFP_FORMAT_VALIDATOR_VALUE];
}

static void* ffp_read_value(const char* value_string) {
    if (!ffp_file_bound()) {
        fprintf(stderr, "[FFP] File not bound to read from");
        return NULL;
    }

    rewind(ffp_file);

    // For now we will brute force it by reading the entire file
    // and checking for the valuw_string 

    if (!ffp_file_bound()) {
        fprintf(stderr,"[FFP] File not bound to read from after rewind");
        return NULL;
    }

    char line[FFP_MAX_LINE_LENGTH];
    long long unsigned int lineNum = 1; 

    while (fgets(line, sizeof(line), ffp_file) != NULL) {
        if (!line) continue;

        int valid = ffp_validate_line_format(line);

        if (!valid) {
            fprintf(stderr,"[FFP] Invalid line format at line %lld\n", lineNum);
            continue;
        }

        char* field = ffp_get_field_from_line(line);

        if (str_compare(value_string, field)) {
            // printf("Field \'%s\', found\n", field);

            return (void*)ffp_get_value_from_line(line);
        }

        lineNum++;
    }
    
    return NULL; // Not found
}

char* ffp_read_string(const char* value_string) {
    return ffp_read_value(value_string);
}

int ffp_read_int(const char* value_string) {
    return ascii_to_int(ffp_read_value(value_string));
}

float ffp_read_float(const char* value_string) {
    return ascii_to_float(ffp_read_value(value_string));
}

#pragma endregion scan
#pragma endregion impl

#endif /* FFP_IMPL */

#endif /* FFP_INCLUDED */