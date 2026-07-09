#include "FFP_Scan.h"

static int ffp_validate_line_format(const char* line) {
    // Format:
    //
    // field=value   -> integer
    // field="value" -> string

    return 1;
} 

static char* ffp_get_field_from_line(const char* line) {
    return "Field";
}

char* ffp_read_value(const char* value_string) {
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
        char *fieldLine = trim_outer_string(line);
        
        if (!fieldLine) continue;

        int valid = ffp_validate_line_format(fieldLine);

        if (!valid) {
            fprintf(stderr,"Invalid line format at line %d\n", lineNum);
            continue;
        }

        char* field = ffp_get_field_from_line(fieldLine);

        if (str_compare(value_string, field)) {
            printf("Field \'%s\', found\n", field);
        }

        lineNum++;
    }
}