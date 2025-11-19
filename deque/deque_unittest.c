#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/deque.h"
#include "../include/unittest.h"

FILE *DEBUG_OUT = NULL;

int main(int argc, char **argv) {
    test tests[] = {
        
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}