#include <cs/stack.h>

#include "../include/unittest.h"

FILE *DEBUG_OUT = NULL;

int main(int argc, char **argv) {
    test tests[] = {

    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}