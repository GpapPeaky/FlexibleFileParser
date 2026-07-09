#include "FFP_Utils.h"
#include "FFP_File.h"
#include "FFP_Scan.h"

int main(void) {
    FILE* f = fopen("test.txt", "r");

    ffp_bind_file(f);

    ffp_read_value("val1=5");

    return 0;
}