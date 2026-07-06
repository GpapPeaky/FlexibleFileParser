#include "FFP_Utils.h"

int is_null(char c) {
    return c == '\0';
}

char* trim_outer_string(const char* str, char delim) {
    if (!str) { perror("Invalid string provided\n"); return NULL; }

    char* construct = (char*)malloc(512); // Max 512 chars
    if (!construct) { perror("Failed to allocate construct\n"); return NULL; }

    size_t len = 0;
    
    int in_inner_str = 0;
    
    // head
    for (size_t i = 0 ; !is_null(str[i]) ; i++) {
        if (str[i] == delim && !in_inner_str) {
            continue;
        }
            
        construct[len] = str[i]; // Copy over
        in_inner_str = 1;
        len++;
    }
        
    if (len == 0) { return NULL; }
    
    size_t j = len - 1;
    
    // trail   
    for ( ; construct[j] == delim ; ) {
        j--;
    }
    
    construct[j + 1] = '\0'; //  Terminate
    
    return construct;
}