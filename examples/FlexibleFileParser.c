#define FFP_IMPL
#include "../lib/FFP.h"

int main(void) {
    FILE* file = fopen("tests/test.txt", "r");

    ffp_bind_file(file);

    char*   s   = ffp_read_string("x");
    int     i   = ffp_read_int("val2");
    float   f   = ffp_read_float("f1");
    double  pi  = ffp_read_float("pi"); 

    if(s) printf("%s\n", s);
        
    printf("%d\n", i);
    printf("%f\n",  f);
    printf("%g\n",  pi);

    return 0;
}