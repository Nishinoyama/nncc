#include <stdio.h>
#include "nncc.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        nncc("a = 10; b = 20; a + b;");
    } else if (argc == 2) {
        nncc(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s code\n", argv[0]);
        return 1;
    }
}

