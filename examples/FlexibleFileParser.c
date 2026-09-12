#define __FFP_IMPL__
#include "../lib/FFP.h"

int main(void) {
    FILE* f = fopen("tests/test.txt", "r");

    ffp_bind_file(f);

    char* p = ffp_read_value("x");
    int* p2 = ffp_read_value("val2");

    if(p)
        printf("%s\n", p);

    if(p2)
        printf("%s\n", p2);

    return 0;
}