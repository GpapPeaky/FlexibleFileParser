#include "FFP_File.h"

FILE* ffp_file = NULL; // Initialize to null

void ffp_bind_file(FILE* file) {
    ffp_file = file;

    if (!ffp_file_bound()) {
        fprintf(stderr,"Failed to bind file");

        return;
    }

    char checksum[3];

    fscanf(ffp_file, "%s", checksum);

    if (!str_compare(checksum, "FFP")) {
        fprintf(stderr,"Invalid file format, file needs to be specified as FFP at line 1");
        
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