#include "FFP_Scan.h"

static int ffp_validate_line_format(const char* line) {
    // Format:
    //
    // field=value   -> integer
    // field="value" -> string

    char* trimmed = trim_outer_string(line);

    // printf("LINE %s\n", line);
    // printf("TRIMMED %s\n", trimmed);

    char* compressed = malloc(sizeof(char) * 512);

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
        tokens[j] = malloc(sizeof(char) * 512);
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

    char* compressed = malloc(sizeof(char) * 512);

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
        tokens[j] = malloc(sizeof(char) * 512);
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

    char* compressed = malloc(sizeof(char) * 512);

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
        tokens[j] = malloc(sizeof(char) * 512);
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
    rewind(ffp_file);

    // For now we will brute force it by reading the entire file
    // and checking for the valuw_string 

    if (!ffp_file_bound()) {
        fprintf(stderr,"File not bound to read from");
        return value_string;
    }

    char line[FFP_MAX_LINE_LENGTH];
    long long unsigned int lineNum = 1; 

    while (fgets(line, sizeof(line), ffp_file) != NULL) {
        if (!line) continue;

        int valid = ffp_validate_line_format(line);

        if (!valid) {
            fprintf(stderr,"Invalid line format at line %d\n", lineNum);
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