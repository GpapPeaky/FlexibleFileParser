#include "FFP_File.h"

FILE* ffp_file = NULL; // Initialize to null

void ffp_bind_file(FILE* file) {
    ffp_file = file;
}

void ffp_unbind_file(void) {
    ffp_file = NULL;
}

int ffp_file_bound(void) {
    return ffp_file != NULL;
}