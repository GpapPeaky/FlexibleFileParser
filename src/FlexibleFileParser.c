#include "FFP_Utils.h"
#include "FFP_File.h"

int main(void) {
    char* str = trim_outer_string("", 'c');

    if (str == NULL) perror("NULL string\n");

    FILE* f = fopen("test.txt", "r");

    ffp_bind_file(f);
    
    printf("%d\n", ffp_file_bound());
    
    ffp_unbind_file();
    
    printf("%d\n", ffp_file_bound());

    // bind tests ^

    ffp_bind_file(f);
    
    printf("%d\n", ffp_file_bound());

    return 0;
}