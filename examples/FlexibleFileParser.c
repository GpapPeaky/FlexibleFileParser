#define __FFP_IMPL__
#include "../lib/FFP.h"

int main(void) {
    FILE* f = fopen("tests/test.txt", "r");

    ffp_bind_file(f);

    char* p = ffp_read_value("x");

    if(p)
        printf("%s\n", p);

    return 0;
}