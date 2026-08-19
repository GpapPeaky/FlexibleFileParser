#include "FFP_Utils.h"
#include "FFP_File.h"
#include "FFP_Scan.h"

int main(void) {
    FILE* f = fopen("test.txt", "r");

    ffp_bind_file(f);

    char* p = ffp_read_value("x");

    if(p)
        printf("%s\n", p);

    return 0;
}